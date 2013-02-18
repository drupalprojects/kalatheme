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
  // Responsive style plugin settings
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

  // Panels styles style plugin settings
  $form['pane_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pane Styles'),
    '#weight' => 43,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('If toggled on, the kalacustomize style plugin will allow the user to set a class for panels panes.'),
     
  );
  $form['pane_styles']['pane_styles_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use panels styles.'),
    '#default_value' => theme_get_setting('pane_styles_toggle'),
    '#description' => t('Check here if you want to set the class for each panels pane.')
  );
  $form['pane_styles']['pane_styles_settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="pane_styles_toggle"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Extra styles style plugin settings
  $form['extra_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra Styles'),
    '#weight' => 44,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('If toggled on, the kalacustomize style plugin will allow the user to set elements and classes for panels panes titles and content.'),
  );
  $form['extra_styles']['extra_styles_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use extra styles.'),
    '#default_value' => theme_get_setting('extra_styles_toggle'),
    '#description' => t('Check here if you want to customize the elements and classes for pane titles and content.')
  );
  $form['extra_styles']['extra_styles_settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="extra_styles_toggle"]' => array('checked' => FALSE),
      ),
    ),
  );


}
