<?php
/**
 * @file
 * Theme and hook functions for fapi things.
 */

/**
 * Implements theme_button().
 */
function kalatheme_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'btn-primary';
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';;
}

/**
 * Implements theme_textarea().
 */
function kalatheme_textarea($variables) {
  $element = $variables['element'];
  _kalatheme_ensure_class($element);
  $element['#attributes']['name'] = $element['#name'];
  $element['#attributes']['id'] = $element['#id'];
  $element['#attributes']['cols'] = $element['#cols'];
  $element['#attributes']['rows'] = $element['#rows'];
  _form_set_class($element, array('form-textarea'));

  // Add Bootstrap class.
  $element['#attributes']['class'][] = 'form-control';

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    $wrapper_attributes['class'][] = 'resizable';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

/**
 * Implements theme_textfield().
 */
function kalatheme_textfield($variables) {
  _kalatheme_ensure_class($variables['element']);
  // Add Bootstrap class, then defer to core for the rest.
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_textfield($variables);
}

/**
 * Implements theme_select().
 */
function kalatheme_select($variables) {
  _kalatheme_ensure_class($variables['element']);
  // Add Bootstrap class, then defer to core for the rest.
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_select($variables);
}

/**
 * Implements theme_password().
 */
function kalatheme_password($variables) {
  _kalatheme_ensure_class($variables['element']);
  // Add Bootstrap class, then defer to core for the rest.
  $variables['element']['#attributes']['class'][] = 'form-control';
  return theme_password($variables);
}

/**
 * Checks that a render array's ['#attribute']['class'] is set, adding it if
 * necessary.
 *
 * @param array $render_array
 *   A renderable array.
 */
function _kalatheme_ensure_class(&$render_array) {
  if (!isset($render_array['#attributes'])) {
    $render_array['#attributes'] = array();
  }
  if (!isset($render_array['#attributes']['class'])) {
    $render_array['#attributes']['class'] = array();
  }
}
