<?php
/**
 * @file
 * Theme and hook functions for views.
 */

// Load plugins
// We need to do this so that our views plugin class will be loaded correctly
// since we cant use the files[] directive in a theme .info file
require_once dirname(__FILE__) . '/../views/plugins/views_plugin_style_grid_bootstrap.inc';

/**
 * Implement hook_views_plugins_alter()
 *
 * We are using this so we can add some responsive Bootstrap options to
 * views grid displays
 */
function kalatheme_views_plugins_alter(&$plugins) {
  if (isset($plugins['style']['grid'])) {
    $plugins['style']['grid']['handler'] = 'views_plugin_style_grid_bootstrap';
    $plugins['style']['grid']['path'] = drupal_get_path('theme', 'kalatheme') . '/views/plugins';
    $plugins['style']['grid']['file'] = 'views_plugin_style_grid_bootstrap.inc';
  }
  $plugins = $plugins;
}

/**
 * Implements hook_preprocess_views_view_grid().
 */
function kalatheme_preprocess_views_view_grid(&$variables) {
  $variables['gridsize'] = kalatheme_get_grid_size();
  $responsive_tiers = array('xs', 'sm', 'lg');
  foreach ($responsive_tiers as $tier) {
    if (empty($variables['options']['columns_' . $tier])) {
      $variables['options']['columns_' . $tier] = 1;
    }
    if ($variables['gridsize'] % $variables['options']['columns_' . $tier] === 0) {
      $variables[$tier] = $variables['gridsize'] / $variables['options']['columns_' . $tier];
    }
  }
  // This is set using the default grid columns value not columns_tier
  if ($variables['gridsize'] % $variables['options']['columns'] === 0) {
    $variables['md'] = $variables['gridsize'] / $variables['options']['columns'];
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
