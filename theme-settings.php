<?php

/**
 * @file
 * Theme setting callbacks for kalatheme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function kalatheme_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['responsive'] = array(
    '#type' => 'fieldset',
    '#title' => t('Responsive'),
    '#weight' => 42,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('If toggled on, the kalacustomize style plugin will allow the user to configure pane device visibilty.'),
  );
  $form['responsive']['responsive_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use responsive toggling.'),
    '#default_value' => theme_get_setting('responsive_toggle'),
    '#description' => t('Check here if you want the user to be able to set the responsive visbility of each panels pane.')
  );

  /*
  $form['pane_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pane Styles'),
    '#weight' => 42,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['pane_styles']['allowed_values'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed values list'),
    '#default_value' => list_allowed_values_string($settings['allowed_values']),
    '#rows' => 10,
    '#element_validate' => array('list_allowed_values_setting_validate'),
    '#field_has_data' => $has_data,
    '#field' => $field,
    '#field_type' => $field['type'],
    '#access' => empty($settings['allowed_values_function']),
  );

  $description = '<p>' . t('The possible values this field can contain. Enter one value per line, in the format key|label.');
  */
}
