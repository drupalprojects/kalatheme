<?php
/**
 * @file
 * Kalatheme's primary magic delivery system (MDS).
 */

// Use DIRNAME instead of drupal_get_path so we can use this
// during an install profile without nuking the world
// Load some helper functions
require_once dirname(__FILE__) . '/includes/utils.inc';

// Asset stuff
define('KALATHEME_BOOTSTRAP_CSS', '//netdna.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css');
define('KALATHEME_BOOTSTRAP_JS', '//netdna.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js');
define('KALATHEME_FONTAWESOME_CSS', '//maxcdn.bootstrapcdn.com/font-awesome/4.1.0/css/font-awesome.min.css');

// Grid size constants
define('KALATHEME_GRID_SIZE', kalatheme_get_grid_size());
define('KALATHEME_GRID_FULL', 1);
define('KALATHEME_GRID_HALF', 1/2);
define('KALATHEME_GRID_THIRD', 1/3);
define('KALATHEME_GRID_FOURTH', 1/4);
define('KALATHEME_GRID_FIFTH', 1/5);
define('KALATHEME_GRID_SIXTH', 1/6);
// Just because we can
define('KALATHEME_GRID_SILLY', 1/42);

// Load Bootstrap overrides of Drupal theme things
require_once dirname(__FILE__) . '/includes/core.inc';
require_once dirname(__FILE__) . '/includes/fapi.inc';
require_once dirname(__FILE__) . '/includes/fields.inc';
require_once dirname(__FILE__) . '/includes/menu.inc';
require_once dirname(__FILE__) . '/includes/panels.inc';
require_once dirname(__FILE__) . '/includes/views.inc';

/**
 * Implements hook_theme().
 */
function kalatheme_theme($existing, $type, $theme, $path) {
  return array(
    'menu_local_actions' => array(
      'variables' => array('menu_actions' => NULL, 'attributes' => NULL),
      'file' => 'includes/menu.inc',
    ),
  );
}

/**
 * Remove conflicting CSS.
 *
 * Implements hook_css_alter().
 */
function kalatheme_css_alter(&$css) {
  // Unset some panopoly css.
  $panopoly_admin_path = drupal_get_path('module', 'panopoly_admin');
  if (isset($css[$panopoly_admin_path . '/panopoly-admin.css'])) {
    unset($css[$panopoly_admin_path . '/panopoly-admin.css']);
  }
  $panopoly_magic_path = drupal_get_path('module', 'panopoly_magic');
  if (isset($css[$panopoly_magic_path . '/css/panopoly-modal.css'])) {
    unset($css[$panopoly_magic_path . '/css/panopoly-modal.css']);
  }
  // Unset some core css.
  unset($css['modules/system/system.menus.css']);
}

/**
 * Implements hook_js_alter().
 *
 * Borrowed from Radix :)
 *
 */
function kalatheme_js_alter(&$javascript) {
  // Add kalatheme-modal only when required.
  $ctools_modal = drupal_get_path('module', 'ctools') . '/js/modal.js';
  $kalatheme_modal = drupal_get_path('theme', 'kalatheme') . '/js/kalatheme-modal.js';
  if (!empty($javascript[$ctools_modal]) && empty($javascript[$kalatheme_modal])) {
    $javascript[$kalatheme_modal] = array_merge(
      drupal_js_defaults(), array('group' => JS_THEME, 'data' => $kalatheme_modal));
  }
}

/**
 * Load Kalatheme dependencies.
 *
 * Implements template_preprocess_html().
 */
function kalatheme_preprocess_html(&$variables) {
  // Load all dependencies.
  _kalatheme_load_dependencies();
  // Add variables for path to theme.
  $variables['base_path'] = base_path();
  $variables['path_to_kalatheme'] = dirname(__FILE__);
  // Add meta for Bootstrap Responsive.
  // <meta name="viewport" content="width=device-width, initial-scale=1.0">
  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0',
    ),
  );
  drupal_add_html_head($element, 'bootstrap_responsive');
}

/**
 * Override or insert variables into the page template.
 *
 * Implements template_process_page().
 */
function kalatheme_process_page(&$variables) {
  // Add Bootstrap JS and stock CSS.
  global $base_url;
  $base = parse_url($base_url);
  // Use the CDN if not using libraries
  if (!kalatheme_use_libraries()) {
    $library = theme_get_setting('bootstrap_library');
    if ($library !== 'none' && !empty($library)) {
      // Add the JS. Note that we have to put Bootstrap after jQuery, but before jQuery UI.
      $url = $base['scheme'] . ":" . KALATHEME_BOOTSTRAP_JS;
      $jquery_ui_library = drupal_get_library('system', 'ui');
      $jquery_ui_js = reset($jquery_ui_library['js']);
      drupal_add_js($url, array(
        'type' => 'external',
        'group' => JS_LIBRARY,
        'weight' => $jquery_ui_js['weight'] - 1,
      ));

      // Add the CSS
      if ($library == 'default') {
        $css = $base['scheme'] . ':' . KALATHEME_BOOTSTRAP_CSS;
      }
      else {
        $css = kalatheme_get_bootswatch_theme($library)->cssCdn;
      }
      drupal_add_css($css, 'external');
    }
  }
  // Use Font Awesome
  if (theme_get_setting('fontawesome')) {
    drupal_add_css($base['scheme'] . ":" . KALATHEME_FONTAWESOME_CSS, 'external');
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
 * Override or insert variables into the page template at a later stage compared
 * to template_process_page()
 *
 * Implements template_preprocess_page().
 */
function kalatheme_preprocess_page(&$variables) {
  // Get the menu tree for the menu that is set as 'Source for the Main links'.
  $main_links_menu = variable_get('menu_main_links_source', 'main-menu');
  $main_menu_tree = menu_tree_all_data($main_links_menu, NULL, 2);
  
  // Add the rendered output to the $main_menu_expanded variable.
  $main_menu_expanded = menu_tree_output($main_menu_tree);

  // Prepare the primary_nav
  $pri_attributes = array(
    'class' => array(
      'nav',
      'navbar-nav',
      'links',
      'clearfix',
    ),
  );
  if (!$variables['main_menu']) {
    $pri_attributes['class'][] = 'sr-only';
  }
  $variables['primary_nav'] = array(
    '#theme' => 'links__system_main_menu',
    '#links' => $main_menu_expanded,
    '#attributes' => $pri_attributes,
    '#heading' => array(
      'text' => t('Main menu'),
      'level' => 'h2',
      'class' => array('sr-only'),
    ),
  );

  // Prepare the secondary_nav
  $sec_attributes = array(
    'id' => 'secondary-menu-links',
    'class' => array('nav', 'navbar-nav', 'secondary-links'),
  );
  if (!$variables['secondary_menu']) {
    $sec_attributes['class'][] = 'sr-only';
  }
  $variables['secondary_nav'] = array(
    '#theme' => 'links__system_secondary_menu',
    '#links' => $variables['secondary_menu'],
    '#attributes' => $sec_attributes,
    '#heading' => array(
      'text' => t('Secondary menu'),
      'level' => 'h2',
      'class' => array('sr-only'),
    ),
  );
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
    $variables['title_attributes_array']['class'][] = 'sr-only';
  }
}
