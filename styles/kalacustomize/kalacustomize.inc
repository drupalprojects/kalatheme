<?php

/**
 * @file
 * Definition of the 'kalacustomize' panel style.
 */

$plugin = array(
  'title' => t('Kalacustomize'),
  'description' => t('Customize elements, visbility and classes with Twitter Bootstrap'),
  'render pane' => 'kalatheme_kalacustomize_render_pane',
  'pane settings form' => 'kalatheme_kalacustomize_pane_settings_form',
  'weight' => 100,
);

/**
 * Theme function for the pane style.
*/
function theme_kalatheme_kalacustomize_render_pane($vars) {
  $content = &$vars['content'];
  $settings = $vars['settings'];

  $content->css_class .= _kalatheme_get_responsive($vars['settings']);

  if (!empty($vars['settings']['pane_style'])) {
    $content->css_class .= $vars['settings']['pane_style'] ? ' ' . $vars['settings']['pane_style'] : '';
  }

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
 * Options for the Panels style plugin to help style panes.
 */
function kalatheme_kalacustomize_pane_settings_form($style_settings) {

  if (theme_get_setting('responsive_toggle')) {
    $mobile_options = array(t('Desktop'), t('Tablet'), t('Phone'));
    $form['devices'] = array(
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
      '#type' => 'checkboxes',
      '#options' => drupal_map_assoc($mobile_options),
      '#title' => t('Hide this pane on the following devices.'),
      '#default_value' => $style_settings['devices'],
    );
  }

  if (theme_get_setting('pane_styles_toggle')) {
    $pane_style_default_value = isset($style_settings['pane_style']) ? $style_settings['pane_style'] : '';
    $form['pane_style'] = array(
      '#title' => t('Pane Style'),
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
    ) + _kalatheme_wrapper_pane_options($pane_style_default_value);
  }

  if (theme_get_setting('extra_styles_toggle')) {
    $element_default_value = isset($style_settings['title']['element']) ? $style_settings['title']['element'] : '';
    $form['title']['element'] = array(
      '#title' => t('Title'),
      '#description' => t('HTML element to wrap around the title.'),
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
    ) + _kalatheme_wrapper_element_options($element_default_value);

    $attribute_default_value = isset($style_settings['title']['attributes']) ? $style_settings['title']['attributes'] : array();
    $form['title']['attributes'] = array(
      '#title' => t('Title Style'),
      '#description' => t('Style to use for this element.'),
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
    ) + _kalatheme_wrapper_attribute_options($attribute_default_value);

    // Content form items, grabbed from the region settings.
    $form += kalatheme_kalacustomize_region_settings_form($style_settings);
  }

  return $form;
}

/**
 * Region settings form callback.
 */
function kalatheme_kalacustomize_region_settings_form($style_settings) {
  $element_default_value = isset($style_settings['content']['element']) ? $style_settings['content']['element'] : '';
  $form['content']['element'] = array(
    '#title' => t('Content'),
    '#description' => t('HTML element to wrap around the content.'),
    '#prefix' => '<div class="well">',
    '#suffix' => '</div>',
  ) + _kalatheme_wrapper_element_options($element_default_value);

  $attribute_default_value = isset($style_settings['content']['attributes']) ? $style_settings['content']['attributes'] : array();
  $form['content']['attributes'] = array(
    '#title' => t('Content Style'),
    '#description' => t('Style to use for this element.'),
    '#prefix' => '<div class="well">',
    '#suffix' => '</div>',
  ) + _kalatheme_wrapper_attribute_options($attribute_default_value);

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
  $options['#options'] = array(
    '' => t('- Use default -'),
    '0' => t('- None -'),
    'div' => 'DIV',
    'span' => 'SPAN',
    'h1' => 'H1',
    'h2' => 'H2',
    'h3' => 'H3',
    'blockquote' => 'BLOCKQUOTE',
  );
  if (theme_get_setting('extra_elements')) {
    $options['#options'] += theme_get_setting('extra_elements');
  }

  return $options;
}

/**
 * Element options for settings forms.
 */
function _kalatheme_wrapper_pane_options($default_value = '') {
  $options = array(
    '#type' => 'radios',
    '#default_value' => $default_value,
  );
  
  $options['#options'] = array(
    '' => t('- Use default -'),
    '0' => t('- None -'),
    'hero-unit' => 'HERO UNIT',
    'well' => 'WELL',
  );
  if (theme_get_setting('pane_classes')) {
    $options['#options'] += theme_get_setting('pane_classes');
  }
  
  return $options;
}

/**
 * Element options for settings forms.
 */
function _kalatheme_wrapper_attribute_options($default_value = '') {
  $options = array(
    '#type' => 'select',
    '#default_value' => $default_value,
  );

  $options['#options'] = array(
    '' => t('- Use default -'),
    '0' => t('- None -'),
    'lead' => 'LEAD BODY COPY',
    'page-header' => 'PAGE HEADER',
    'muted' => 'MUTED TEXT',
  );
  
  if (theme_get_setting('extra_classes')) {
    $options['#options'] += theme_get_setting('extra_classes');
  }

  return $options;
}

/**
 * Create a pane wrapper from provided attributes.
 *
 * @return array
 *   Array with prefix and suffix keys.
 */
function _kalatheme_wrapper_wrap($item = array()) {
  $wrap['prefix'] = NULL;
  $wrap['suffix'] = NULL;
  $wrap['attributes'] = NULL;

  if (!empty($item['attributes'])) {
    $value = $item['attributes'];
    $wrap['attributes'] .= " class=\"$value\"";
  }

  if (!empty($item['element']) && $item['element'] != 'no_wrapper') {
    $wrap['prefix'] = '<' . $item['element'] . $wrap['attributes'] . '>';
    $wrap['suffix'] = '</' . $item['element'] . '>';
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

/**
 * Create responsive display classes depending on chosen devices.
 *
 * @return array
 *   Array with prefix and suffix keys.
 */
function _kalatheme_get_responsive($item = array()) {
  $count = 0;
  foreach ($item['devices'] as $device => $hidden) {
    if ($hidden === 0) {
      $show = strtolower($device);
      $count++;
    }
    else {
      $hide = strtolower($device);
    }
  }

  switch ($count) {
    case 0:
      return ' element-invisible';

    case 1:
      return ' visible-' . $show;

    case 2:
      return ' hidden-' . $hide;

    case 3:
      return '';

  }
}
