<?php
/**
 * @file
 * Theme and hook functions for panels things.
 */

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
 * Implements template_preprocess_panels_pane().
 */
function kalatheme_preprocess_panels_pane(&$variables) {
  $pane = $variables['pane'];
  if (isset($pane->css['css_class'])) {
    // Check for Bootstrap panel classes.
    $css_classes = explode(" ", $pane->css['css_class']);
    $bootstrap_panel_classes = kalatheme_get_bootstrap_panel_classes();
    $panel_classes = array_intersect($css_classes, $bootstrap_panel_classes);

    if (count($panel_classes)) {
      // Use a different template.
      $variables['theme_hook_suggestions'][] = 'panels_pane__bootstrap_panel';

      // Add default classes to panel.
      $variables['classes_array'][] = 'panel';
      $variables['classes_array'] += $panel_classes;

      // Add default classes to pane title.
      $variables['title_attributes_array']['class'][] = 'panel-title';
    }
  }
}

/**
 * Implements template_preprocess_panels_add_content_modal().
 */
function kalatheme_preprocess_panels_add_content_modal(&$variables) {
  // Process the list of categories.
  $variables['categories_array'] = array();
  foreach ($variables['categories'] as $key => $category_info) {
    // 'root' category is actually displayed under the categories, so
    // skip it.
    if ($key == 'root') {
      continue;
    }

    $class = 'panels-modal-add-category btn btn-sm btn-default';
    if ($key == $variables['category']) {
      $class .= ' active';
    }

    $url = $variables['renderer']->get_url('select-content', $variables['region'], $key);
    $variables['categories_array'][] = ctools_ajax_text_button($category_info['title'], $url, '', $class);
  }

  // Now render the top level buttons (aka the root category) if any.
  $variables['root_content'] = '';
  if (!empty($variables['categories']['root'])) {
    foreach ($variables['categories']['root']['content'] as $content_type) {
      $variables['root_content_array'][] = theme('panels_add_content_link', array('renderer' => $variables['renderer'], 'region' => $variables['region'], 'content_type' => $content_type));
    }
  }
}

/**
 * Implements template_process_panels_add_content_modal().
 */
function kalatheme_process_panels_add_content_modal(&$vars) {
  $content = !empty($vars['categories'][$vars['category']]['content']) ? $vars['categories'][$vars['category']]['content'] : array();

  // If no category is selected or the category is empty or our special empty
  // category render a 'header' that will appear instead of the columns.
  if (empty($vars['category']) || empty($content) || $vars['category'] == 'root') {
    // Nothing to do since there is no preview
  }
  else {
    $titles = array_keys($content);
    natcasesort($titles);
    $col_size = count($titles) / $vars['column_count'];

    // Zero out the existing column data
    $count = 0;
    foreach ($titles as $title) {
      $which = floor($count++ / $col_size) + 1;
      $vars['columns'][$which] = '';
    }

    // Read the column data with our preview functionality
    $count = 0;
    foreach ($titles as $title) {
      $which = floor($count++ / $col_size) + 1;

      // Add an 'Add' button to widget.
      $content[$title]['title'] = t('Add');
      $widget_title = theme('panels_add_content_link', array(
        'renderer' => $vars['renderer'],
        'region' => $vars['region'],
        'content_type' => $content[$title],
      ));

      // Render the widget preview.
      $widget_preview = (!empty($content[$title]['preview']) ? $content[$title]['preview'] : t('No Preview'));

      // Render the preview output as a fieldset.
      $preview_output = '<fieldset class="widget-preview panel panel-default">';
      $preview_output .= '<legend class="panel-heading">';
      $preview_output .= '<div class="panel-title fieldset-legend pull-left">' . $title . '</div>';
      $preview_output .= $widget_title;
      $preview_output .= '</legend>';
      $preview_output .= '<div class="panel-body">';
      $preview_output .= $widget_preview;
      $preview_output .= '</div>';
      $preview_output .= '</fieldset>';

      $vars['columns'][$which] .= $preview_output;
    }
  }
}

/**
 * Implements hook_preprocess_panels_add_content_link().
 */
function kalatheme_preprocess_panels_add_content_link(&$vars) {
  $vars['text_button'] = ctools_ajax_text_button($vars['title'], $vars['url'], $vars['description'], 'panels-modal-add-config btn btn-default');
}

/**
 * Returns Bootstrap panel classes.
 */
function kalatheme_get_bootstrap_panel_classes() {
  return array(
    'panel-default',
    'panel-primary',
    'panel-success',
    'panel-info',
    'panel-warning',
    'panel-danger'
  );
}

