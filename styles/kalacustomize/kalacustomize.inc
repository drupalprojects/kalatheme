<?php

/**
 * @file
 * Definition of the 'kalacustomize' panels style plugin.
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
 * Theme fuction for the region style.
 */
function theme_kalatheme_kalacustomize_render_region($vars) {
  $content = $vars['panes'];
  $settings = $vars['settings'];

  // Theme.
  if (!empty($settings['theme']) && $settings['theme']) {
    $output = theme('panels_default_style_render_region', $vars);
  }
  else {
    $output = implode(PHP_EOL, $content);
  }

  // Grab settings and set some defaults
  $settings['content']['element'] = 'div';
  $settings['content']['attributes'] = '';

  if (!empty($vars['settings']['region_style'])) {
    $settings['content']['attributes'] .= $vars['settings']['region_style'] ? ' ' . $vars['settings']['region_style'] : '';
  }
  $settings['content']['attributes'] .= _kalatheme_get_responsive($vars['settings']);

  if (!empty($output)) {
    $content_wrapper = _kalatheme_wrapper_wrap(!empty($settings['content']) ? $settings['content'] : array());
    $output = $content_wrapper['prefix'] . $output . $content_wrapper['suffix'];
  }

  return $output;
}

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
    $content->title = $title_wrapper['prefix'] . $content->title . $title_wrapper['suffix'];
  }
  if (!empty($content->content)) {
    $content_wrapper = _kalatheme_wrapper_wrap(!empty($settings['content']) ? $settings['content'] : array());
    $content->content = $content_wrapper['prefix'] . render($content->content) . $content_wrapper['suffix'];
  }

  $output = theme('panels_pane', $vars);

  return $output;
}

/**
 * Options for the Panels style plugin to help style panes.
 */
function kalatheme_kalacustomize_pane_settings_form($style_settings) {

  $settings_exposed = 0;

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
    $settings_exposed++;
  }

  if (theme_get_setting('pane_styles_toggle')) {
    $pane_style_default_value = isset($style_settings['pane_style']) ? $style_settings['pane_style'] : '';
    $form['pane_style'] = array(
      '#title' => t('Pane Style'),
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
    ) + _kalatheme_wrapper_pane_options($pane_style_default_value);
    $settings_exposed++;
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

    // Content form items, grabbed from the more settings.
    $form += kalatheme_kalacustomize_more_settings_form($style_settings);

    $settings_exposed++;
  }

  // If we don't have any enabled options, let the user know.
  if ($settings_exposed == 0) {
    _kalatheme_warn_no_style_options();
  }

  return $form;
}

/**
 * Region settings form callback.
 */
function kalatheme_kalacustomize_region_settings_form($style_settings) {
  $settings_exposed = 0;

  if (theme_get_setting('responsive_toggle')) {
    $mobile_options = array(t('Desktop'), t('Tablet'), t('Phone'));
    $form['devices'] = array(
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
      '#type' => 'checkboxes',
      '#options' => drupal_map_assoc($mobile_options),
      '#title' => t('Hide this region on the following devices.'),
      '#default_value' => $style_settings['devices'],
    );
    $settings_exposed++;
  }

  if (theme_get_setting('region_styles_toggle')) {
    $region_style_default_value = isset($style_settings['region_style']) ? $style_settings['region_style'] : '';
    $form['region_style'] = array(
      '#title' => t('Region Style'),
      '#prefix' => '<div class="well">',
      '#suffix' => '</div>',
    ) + _kalatheme_wrapper_region_options($region_style_default_value);
    $settings_exposed++;
  }

  // If we don't have any enabled options, let the user know.
  if ($settings_exposed == 0) {
    _kalatheme_warn_no_style_options();
  }

  return $form;
}

/**
 * More settings form callback.
 */
function kalatheme_kalacustomize_more_settings_form($style_settings) {
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
function _kalatheme_wrapper_region_options($default_value = '') {
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
  if (theme_get_setting('region_classes')) {
    $options['#options'] += theme_get_setting('region_classes');
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

/**
 * Adds a warning status message letting the user know that no customization
 * options have been enabled from the theme settings page.
 */
function _kalatheme_warn_no_style_options() {
  $settings_link = l(t('Kalatheme settings page'), 'admin/appearance/settings/kalatheme');
  drupal_set_message(t('You haven\'t enabled any customizations! Please check
    out the !settings_link to see what\'s available.', array('!settings_link' => $settings_link)), 'warning');
}
