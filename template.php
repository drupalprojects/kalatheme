<?php
/**
 * @file
 * Kalatheme's primary theme functions and alterations.
 */

$kalatheme_path = drupal_get_path('theme', 'kalatheme');
require_once $kalatheme_path . '/includes/theme.inc';

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
  require_once DRUPAL_ROOT . '/' . $variables['path_to_kalatheme'] . '/includes/kalatheme.inc';
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
  $variables['no_panels'] = FALSE;
  if (!isset($variables['page']['content']['system_main']['main']['#markup']) || (strpos($variables['page']['content']['system_main']['main']['#markup'], 'panel-panel') === FALSE)) {
    $variables['no_panels'] = TRUE;
  }
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
  $vars['text_button'] = ctools_ajax_text_button($vars['title'], $vars['url'], $vars['description'], 'panels-modal-add-config btn');
}

/**
 * Implements hook_preprocess_views_view_grid().
 */
function kalatheme_preprocess_views_view_grid(&$variables) {
  if (12 % $variables['options']['columns'] === 0) {
    $variables['span'] = 'span' . 12 / $variables['options']['columns'];
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
}

/**
 * Checks if Bootstrap's responsive CSS is installed.
 *
 * @param array $variant
 *   Library, or one of its variants, to check
 * @param string $version
 *   Library's version number, if applicable
 * @param string $variant_name
 *   Name of current variant, if applicable
 */
function kalatheme_check_responsive(&$variant, $version, $variant_name) {
  foreach (array_keys($variant['files']['css']) as $css) {
    if (!preg_match('/^css\/bootstrap\-responsive\.(?:min\.)?css$/', $css)) {
      continue;
    }
    $css_path = DRUPAL_ROOT . '/' . $variant['library path'] . '/' . $css;
    if (!file_exists($css_path)) {
      unset($variant['files']['css'][$css]);
    }
  }
}

/**
 * Implements hook_libraries_info().
 */
function kalatheme_libraries_info() {
  $libraries = array();
  $libraries['bootstrap'] = array(
    'name' => 'Twitter Bootstrap',
    'machine name' => 'bootstrap',
    'vendor url' => 'http://twitter.github.com',
    'download url' => 'http://twitter.github.com',
    'path' => '',
    'callbacks' => array(
      'pre-load' => array(
        'kalatheme_check_responsive',
      ),
    ),
    'version arguments' => array(
      'pattern' => '@v+([0-9a-zA-Z\.-]+)@',
      'lines' => 100,
      'cols' => 200,
    ),
    'version callback' => '_kalatheme_get_version',
    'versions' => array(
      '2' => array(
        'files' => array(
          'js' => array(
            'js/bootstrap.js',
          ),
          'css' => array(
            'css/bootstrap.css',
            'css/bootstrap-responsive.css',
          ),
        ),
        'variants' => array(
          'minified' => array(
            'files' => array(
              'js' => array(
                'js/bootstrap.min.js',
              ),
              'css' => array(
                'css/bootstrap.min.css',
                'css/bootstrap-responsive.min.css',
              ),
            ),
            'variant arguments' => array(
              'variant' => 'minified',
            ),
          ),
        ),
      ),
    ),
  );
  
  return $libraries;
}
