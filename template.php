<?php
/**
 * @file
 * Kalatheme's primary pre/preprocess functions and alterations.
 */

// Constants
// We want to use the global here to respect alterations from modules
// like ThemeKey
global $theme_key;
if (!defined('KALATHEME_BOOTSTRAP_LIBRARY')) {
  define('KALATHEME_BOOTSTRAP_LIBRARY', $theme_key . '_bootstrap');
}

// Load some core things
$kalatheme_path = drupal_get_path('theme', 'kalatheme');
require_once $kalatheme_path . '/includes/theme.inc';
require_once $kalatheme_path . '/includes/libraries.inc';

/**
 * Represents the number of columns in the grid supplied by Bootstrap.
 * And provides some common grid sizes
 */
define('KALATHEME_GRID_SIZE', kalatheme_get_grid_size());
define('KALATHEME_GRID_FULL', 1);
define('KALATHEME_GRID_HALF', 1/2);
define('KALATHEME_GRID_THIRD', 1/3);
define('KALATHEME_GRID_FOURTH', 1/4);
define('KALATHEME_GRID_FIFTH', 1/5);
define('KALATHEME_GRID_SIXTH', 1/6);
// Just because we can
define('KALATHEME_GRID_SILLY', 1/42);

/**
 * Implements hook_theme().
 */
function kalatheme_theme($existing, $type, $theme, $path) {
  return array(
    'menu_local_actions' => array(
      'variables' => array('menu_actions' => NULL, 'attributes' => NULL),
      'file' => 'includes/theme.inc',
    ),
  );
}

/**
 * Implements hook_menu() (via hook menu alter because that is how themes roll).
 */
function kalatheme_menu_alter(&$items) {
  $items['admin/appearance/kalasetup'] = array(
    'page callback' => 'drupal_get_form',
    'page arguments' => array('kalatheme_setup_form'),
    'access arguments' => array('administer themes'),
    'weight' => 25,
    'type' => MENU_LOCAL_ACTION,
    'file' => 'setup.inc',
    'file path' => drupal_get_path('theme', 'kalatheme') . '/includes',
    'title' => 'Setup Kalatheme',
  );
}

/**
 * Remove conflicting CSS.
 *
 * Implements hook_css_alter().
 */
function kalatheme_css_alter(&$css) {
  // Pull out some panopoly CSS, will want to pull more later
  unset($css[drupal_get_path('module', 'panopoly_admin') . '/panopoly-admin.css']);
  unset($css[drupal_get_path('module', 'panopoly_magic') . '/css/panopoly-modal.css']);
}

/**
 * Load Kalatheme dependencies.
 *
 * Implements template_preprocess_html().
 */
function kalatheme_preprocess_html(&$variables) {
  // Add variables for path to theme.
  $variables['base_path'] = base_path();
  $variables['path_to_kalatheme'] = drupal_get_path('theme', 'kalatheme');

  // Load all dependencies.
  require_once DRUPAL_ROOT . '/' . $variables['path_to_kalatheme'] . '/includes/utils.inc';
  _kalatheme_load_dependencies();
}

/**
 * Override or insert variables into the page template for HTML output.
 *
 * Implements template_process_html().
 */
function kalatheme_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Override or insert variables into the page template.
 *
 * Implements template_process_page().
 */
function kalatheme_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }

  // Define variables to theme local actions as a dropdown.
  $dropdown_attributes = array(
    'container' => array(
      'class' => array('dropdown', 'actions', 'pull-right'),
    ),
    'toggle' => array(
      'class' => array('dropdown-toggle', 'enabled'),
      'data-toggle' => array('dropdown'),
      'href' => array('#'),
    ),
    'content' => array(
      'class' => array('dropdown-menu'),
    ),
  );

  // Add local actions as the last item in the local tasks.
  if (!empty($variables['action_links'])) {
    $variables['tabs']['#primary'][]['#markup'] = theme('menu_local_actions', array('menu_actions' => $variables['action_links'], 'attributes' => $dropdown_attributes));
    $variables['action_links'] = FALSE;
  }

  // Get the entire main menu tree.
  $main_menu_tree = array();
  $main_menu_tree = menu_tree_all_data('main-menu', NULL, 2);
  // Add the rendered output to the $main_menu_expanded variable.
  $variables['main_menu_expanded'] = menu_tree_output($main_menu_tree);

  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty,
    // so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }

  // If panels arent being used at all.
  $variables['no_panels'] = !(module_exists('page_manager') && page_manager_get_current_page());

  // Check if we're to always print the page title, even on panelized pages.
  $variables['always_show_page_title'] = theme_get_setting('always_show_page_title') ? TRUE : FALSE;
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function kalatheme_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page()
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  drupal_add_css(drupal_get_path('theme', 'kalatheme') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function kalatheme_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, rebuild the empty site slogan.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
}

/*
 * Implements hook_panels_default_style_render_region().
 *
 * Some magic from @malberts with inspiration from Omega
 */
function kalatheme_panels_default_style_render_region(&$variables) {
  // Remove .panels-separator.
  return implode('', $variables['panes']);
}

/**
 * Override or insert variables into the node template.
 *
 * Implements template_preprocess_node().
 */
function kalatheme_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Override or insert variables into the block template.
 *
 * Implements template_preprocess_block().
 */
function kalatheme_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Implements hook_preprocess_panels_add_content_link().
 */
function kalatheme_preprocess_panels_add_content_link(&$vars) {
  $vars['text_button'] = ctools_ajax_text_button($vars['title'], $vars['url'], $vars['description'], 'panels-modal-add-config btn btn-default');
}

/**
 * Implements hook_preprocess_views_view_grid().
 */
function kalatheme_preprocess_views_view_grid(&$variables) {
  if (kalatheme_get_grid_size() % $variables['options']['columns'] === 0) {
    $variables['span'] = 'col-md-' . kalatheme_get_grid_size() / $variables['options']['columns'];
  }
}

/**
 * Implements hook_preprocess_views_view_table().
 */
function kalatheme_preprocess_views_view_table(&$variables) {
  $rows = array();
  foreach ($variables['row_classes'] as $row) {
    // This assume the first element of any row will be the odd/even class which we no longer need
    array_shift($row);
    $rows[] = $row;
  }
  $variables['row_classes'] = $rows;

  // Add in bootstrap classes
  $variables['classes_array'] = array('table', 'table-striped', 'table-bordered', 'table-hover');

  // Remove the active class from table cells, as Bootstrap 3 gives them a funky background.
  $handler = $variables['view']->style_plugin;
  $active = !empty($handler->active) ? $handler->active : FALSE;
  if ($active) {
    foreach ($variables['field_classes'][$active] as &$cell) {
      $cell_classes = explode(' ', $cell);
      $active_class_index = array_search('active', $cell_classes);
      if ($active_class_index !== FALSE) {
        unset($cell_classes[$active_class_index]);
        $cell = implode(' ', $cell_classes);
      }
    }
  }
}
