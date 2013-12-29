<?php

/**
 * @file
 * Kalatheme's subtheme setup stuff
 */

/**
 * Form for kalatheme setup
 */
function kalatheme_setup_form($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Placeholder text for now'),
  );
  // This is the first form element. It's a textfield with a label, "Name"
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Let\'s build a kalatheme!'),
  );
  return $form;
}
