<?php

/**
 * @file
 * Definition of the 'kalacustomize' panel style.
 */

$plugin = array(
  'title' => t('Kalacustomize'),
  'description' => t('Customize elements, visbility and classes with Twitter Bootstrap'),
  'render region' => 'kalatheme_kalacustomize_render_region',
  'render pane' => 'kalatheme_kalacustomize_render_pane',
  'settings form' => 'kalatheme_kalacustomize_region_settings_form',
  'pane settings form' => 'kalatheme_kalacustomize_pane_settings_form',
  'weight' => 100,
);

/**
 * Theme function for the pane style.
 */
function theme_kalatheme_kalacustomize_render_pane($vars) {
  $content = &$vars['content'];
  $settings = $vars['settings'];

  if (!empty($content->title)) {
    $title_wrapper = _kalatheme_wrapper_wrap(!empty($settings['title']) ? $settings['title'] : array());
    $content->title
      = $title_wrapper['prefix'] .
        $content->title .
        $title_wrapper['suffix'];
  }
  if (!empty($content->content)) {
    $content_wrapper = _kalatheme_wrapper_wrap(!empty($settings['content']) ? $settings['content'] : array());
    $content->content
      = $content_wrapper['prefix'] .
        render($content->content) .
        $content_wrapper['suffix'];
  }

  $output = theme('panels_pane', $vars);
  
  return $output;
}

/**
 * Theme function for the region style.
 */
function theme_kalatheme_kalacustomize_render_region($vars) {
  $content = $vars['panes'];
  $settings = $vars['settings'];

  $output = theme('panels_default_style_render_region', $vars);

  if (!empty($output)) {
    $content_wrapper = _panels_extra_styles_wrapper_wrap(!empty($settings['content']) ? $settings['content'] : array());
    $output
      = $content_wrapper['prefix'] .
        $output .
        $content_wrapper['suffix'];
  }

  return $output;
}

/**
 * Some responsive options
 */
function kalatheme_kalacustomize_pane_settings_form($style_settings) {
  
  // Create the responsive toggle form 
  $form['devices'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(t('Desktop'), t('Tablet'), t('Mobile'))),
    '#title' => t('Show this pane on the following devices. By default it will show on all devices.'),
    '#default_value' => $style_settings['devices'],
  );
  
  $element_default_value = isset($style_settings['title']['element']) ? $style_settings['title']['element'] : '';
  $form['title']['element'] = array(
    '#title' => t('Title'),
    '#description' => t('HTML element to wrap around the title.'),
  ) + _kalatheme_wrapper_element_options($element_default_value);

  // Content form items, grabbed from the region settings.
  $form += kalatheme_kalacustomize_region_settings_form($style_settings);

  return $form;
}

/**
 * Region settings form callback.
 */
function kalatheme_kalacustomize_region_settings_form($style_settings) {
  // Content.
  $element_default_value = isset($style_settings['content']['element']) ? $style_settings['content']['element'] : '';
  $form['content']['element'] = array(
      '#title' => t('Content'),
      '#description' => t('HTML element to wrap around the content.'),
  ) + _kalatheme_wrapper_element_options($element_default_value);

  return $form;
}

/**
 * Element options for settings forms.
 */
function _kalatheme_wrapper_element_options($default_value = '') {
  $options = array(
      '#type' => 'select',
      '#default_value' => $default_value,
  );

  // Fallback element options, grabbed from View's defaults.
  $options['#options'] = variable_get('views_field_rewrite_elements', array(
      '' => t('- Use default -'),
      '0' => t('- None -'),
      'div' => 'DIV',
      'span' => 'SPAN',
      'h1' => 'H1',
      'h2' => 'H2',
      'h3' => 'H3',
      'h4' => 'H4',
      'h5' => 'H5',
      'h6' => 'H6',
      'p' => 'P',
      'strong' => 'STRONG',
      'em' => 'EM',
  ));

  return $options;
}

/**
 *
 * @return array
 *   Array with prefix and suffix keys.
 */
function _kalatheme_wrapper_wrap($item = array()) {
  $wrap['prefix'] = NULL;
  $wrap['suffix'] = NULL;
  $wrap['attributes'] = NULL;

  if (!empty($item['attributes'])) {
    foreach ($item['attributes'] as $key => $value) {
      if (!empty($value)) {
        $wrap['attributes'] .= " $key=\"$value\"";
      }
    }
  }

  if (!empty($item['element']) && $item['element'] != 'no_wrapper') {
    $wrap['prefix'] = '<' . $item['element'] . $wrap['attributes'] . '>';
    $wrap['suffix'] = '</' .  $item['element'] . '>';
  }

  if (!empty($item['prefix'])) {
    $wrap['prefix'] = $item['prefix'];
  }
  if (!empty($item['suffix'])) {
    $wrap['suffix'] = $item['suffix'];

  }

  $wrap['prefix'] = PHP_EOL . $wrap['prefix'];
  $wrap['suffix'] = $wrap['suffix'] . PHP_EOL;

  return $wrap;
}
