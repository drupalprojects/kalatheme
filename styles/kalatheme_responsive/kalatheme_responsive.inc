<?php

/**
 * @file
 * Definition of the 'dark blue background' panel style.
 */

$plugin = array(
  'title' => t('Kalatheme Responsive'),
  'description' => t('Leverage responsive Twitter Bootstrap classes'),
  'render pane' => 'kalatheme_kalatheme_responsive_render_pane',
  'pane settings form' => 'kalatheme_kalatheme_responsive_settings_form',
  'weight' => 100,
);

/**
 * Theme function for the pane style.
 */
function theme_kalatheme_kalatheme_responsive_render_pane($vars) {
  $content = $vars['content'];
  $pane = $vars['pane'];
  $display = $vars['display'];
  $plugin = $vars['style'];
  
  return theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));
}

/**
 * Some responsive options
 */
function kalatheme_kalatheme_responsive_settings_form(&$form, &$form_state, $form_id) {
  // Create the responsive toggle form
  $form['devices'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(t('Desktop'), t('Tablet'), t('Mobile'))),
    '#title' => t('Show this pane on the following devices. By default it will show on all devices.'),
    '#default_value' => $form['devices'],
  );
  return $form;
}
