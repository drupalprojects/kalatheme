<?php

/**
 * @file
 * Kalatheme's theme functions for icons
 */

require_once dirname(__FILE__) . '/icons/fontawesome.inc';
require_once dirname(__FILE__) . '/icons/glyphicons.inc';


/**
 * Theme function for icon fonts.
 */
function kalatheme_font_icon($variables){
  $attr = isset($variables['attributes']) ? $variables['attributes'] : array();
  $attr += array('aria-hidden' => 'true' );
  $attr['class'] = array();
  $icon_font_prefix = 'glyicon-';

  if( theme_get_setting('fontawesome') ){
    $icon_font_prefix = 'fa-';
    $attr['class'][] = 'fa';
  }
  else{
    $attr['class'][] = 'glyphicon';
  }

  foreach($variables['keys'] as $class){
    $attr['class'][] = $icon_font_prefix . $class;
  }
  $stub = '<span '. drupal_attributes($attr) . '></span>';
  if(isset($variables['alt_text'])){
    $stub .= '<span class="element-invisible">'.$variables['alt_text'].'</span>';
  }
  return $stub;
}



/**
 * Implements hook_icon_providers().
 */
function kalatheme_icon_providers() {
  $providers['font_awesome'] = array(
    'title' => t('Font Awesome'),
    'url' => 'http://fontawesome.io',
    'default bundle' => array(
      'render' => 'html_tag',
      'settings' => kalatheme_icon_default_settings(),
    ),
  );

  $providers['bootstrap_gyphicons'] = array(
    'title' => t('Bootstrap Gypicons'),
    'url' => 'http://getbootstrap.com/components/#glyphicons',
    'default bundle' => array(
      'render' => 'html_tag',
      'settings' => kalatheme_icon_default_settings(),
    ),
  );
  return $providers;
}

/**
 * Provides the default settings for this
 */
function kalatheme_icon_default_settings(){
  return array(
    'tag' => 'span'
  );
}


/**
 * Implements hook_icon_bundle_configure().
 */
function kalatheme_icon_bundle_configure(&$settings, &$form_state, &$complete_form) {
  $bundle = $form_state['bundle'];
  if ($bundle['provider'] === 'font_awesome' || 'bootstrap_gyphicons') {
    $settings['tag'] = array(
      '#type' => 'select',
      '#title' => t('HTML Markup'),
      '#description' => t('Choose the HTML markup tag that Font Awesome icons should be created with.
        Typically, this is an %span tag, however it can be changed to suite the theme requirements.', array('%span' => '<span/>')),
      '#options' => drupal_map_assoc(array('i', 'span', 'div')),
      '#default_value' => isset($bundle['settings']['tag']) ? $bundle['settings']['tag'] : 'span',
    );
  }
}

/**
 *
 */
function kalatheme_preprocess_icon_html_tag(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  $icon_font_prefix = '';
  if( $bundle === 'font_awesome' ){
    $icon_font_prefix = 'fa-';
  }
  elseif($bundle === 'bootstrap_glyphicon' ){
    $icon_font_prefix = 'glyicon-';
  }
}

/**
 * @desc return HTML tag emement with fontawesome icon classed
 * @param <type> $variables
 * @return <type>
 */
function theme_icon_html_tag($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  $output = theme('html_tag', array(
    'element' => array(
      '#tag' => isset($bundle['settings']['tag']) ? $bundle['settings']['tag'] : 'span',
      '#attributes' => $variables['attributes'],
      '#value' =>'',
    ),
  ));
  return $output;
}



/**
 * @desc return HTML tag emement with fontawesome icon classed
 * @param <type> $variables
 * @return <type>
 */
function kalatheme_icon_html_tag($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  $output = theme('html_tag', array(
    'element' => array(
      '#tag' => isset($bundle['settings']['tag']) ? $bundle['settings']['tag'] : 'span',
      '#attributes' => $variables['attributes'],
      '#value' =>'',
    ),
  ));
  return $output;
}

/**
 * Implements hook_preprocess_icon().
 */
function kalatheme_preprocess_icon(&$variables) {
  $icon = $variables['icon'];
  if (!empty($variables['bundle']['provider'])){
    $classes = array();
    $classes[] = drupal_html_class($icon);
    switch($variables['bundle']['provider']){
      case 'font_awesome':
        $classes[] = 'fa';
        break;
      case 'bootstrap_glyhicons':
        $classes[] = 'glyphicon';
        break;
      default:
        break;
    }
    if( isset($variables['attributes']['class']) ){
      $variables['attributes']['class'] += $classes;
    }else{
      $variables['attributes']['class'] = $classes;
    }
  }
}
/**
 * Implements hook_icon_bundles().
 */
function kalatheme_icon_bundles() {
  $bundles = array();
  $bundles['font_awesome'] = array(
    'render' => 'html_tag',
    'provider' => 'font_awesome',
    'title' => t('Font Awsome'),
    'icons' => _kalatheme_fontawesome_icons()
  );

  $bundles['bootstrap_glyhicons'] = array(
    'render' => 'html_tag',
    'provider' => 'bootstrap_glyhicons',
    'title' => t('Bootstrap Glyphicons'),
    'icons' => _kalatheme_glyphicons_icons()
  );
  return $bundles;
}


/**
 * Define render hook information.
 *
 * @see hook_icon_render_hooks()
 */
function kalatheme_icon_render_hooks() {
  $hooks['html_tag'] = array();
  return $hooks;
}
