<?php

/**
 * @file
 * Kalatheme's theme functions for icons
 */

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
