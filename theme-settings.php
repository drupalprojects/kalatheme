<?php

/**
 * @file
 * Kalatheme's theme settings form page.
 *
 * @todo  I tried to do this a OOP way, but I ran into problems.
 * with the form_state values and calling am alter on a form.
 * I have the code here:
 * https://gist.github.com/labboy0276/e6ed2204f3ea68ca67836bb198ce2a85
 *
 * Another thought is to alter the route of the theme settings route:
 * https://www.drupal.org/node/2187643
 * Then build this form?
 *
 * Another thought would be to render this form as a service and use
 * DI as per: https://www.drupal.org/node/2203931
 *
 * Not sure, been there may be a better way then this or not, who knows.
 */

use Drupal\Core\Form\FormStateInterface;

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Theme settings configuration.
 */
function kalatheme_form_system_theme_settings_alter(&$form, FormStateInterface $form_state) {
  // This adds the veritcal tab to put the global settings in it.
  $form['old'] = [
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2>' . t('Global Settings') . '</h2>',
    '#weight' => 10,
  ];

  // This adds the old group to the settings.
  foreach ($form as $key => &$value) {
    if (isset($value['#type']) && $value['#type'] == 'details') {
      $value['#group'] = 'old';
    }
  }

  // New Kaltheme Settings.
  // Vertical Group Settings.
  $form['kalatheme'] = [
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2>' . t('Kalatheme Settings') . '</h2>',
    '#weight' => -10,
  ];

  // Framework settings detail wrapper.
  $form['framework'] = [
    '#type' => 'details',
    '#title' => t('Framework Settings'),
    '#group' => 'kalatheme',
    '#weight' => 1,
  ];

  // Framework Select list that goes into Config.
  $form['framework']['framework'] = [
    '#type' => 'select',
    '#title' => t('Choose Framework'),
    '#default_value' => theme_get_setting('kalatheme_framework'),
    '#options' => [
      'bootstrap' => 'Bootstrap',
    ],
    '#description' => t('Choose the front end framework'),
  ];

  // Framework configuration detail wrapper.
  $form['framework_config'] = [
    '#type' => 'details',
    '#title' => t('Framework Configuration'),
    '#group' => 'kalatheme',
    '#weight' => 2,
  ];

  // Framework Select list that goes into Config.
  $form['framework_config']['viewport'] = [
    '#type' => 'textfield',
    '#title' => t('Viewport'),
    '#default_value' => theme_get_setting('kalatheme_viewport'),
    '#description' => t('Change the viewport if the need arises'),
  ];


  // Bootstrap CDN Selection.
  $form['framework_config']['bootstrap_cdn'] = [
    '#type' => 'fieldset',
    '#title' => t('Bootstrap CDN Settings'),
    '#states' => [
      'visible' => [
        'select[name="framework_selection"]' => ['value' => 'bootstrap'],
      ],
    ],
  ];

  // Bootstrap CDN Selection Enable that goes into Config.
  // The Outbound link for the decription as well.
  $bsdesc = t('The CDN is obtained from:
    <a href="@bs" target="_blank">Get Bootstrap Page</a>', [
    '@bs' => 'http://getbootstrap.com/getting-started/#download-cdn'
  ]);

  $form['framework_config']['bootstrap_cdn']['bootstrap_enable_cdn'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable Bootstap CDN?'),
    '#default_value' => theme_get_setting('kalatheme_bootstrap_enable_cdn'),
    '#description' => $bsdesc,
    '#states' => [
      'visible' => [
        'select[name="framework"]' => ['value' => 'bootstrap'],
      ],
    ],
  ];

  // Bootstrap CDN CSS URL.
  $form['framework_config']['bootstrap_cdn']['bootstrap_cdn_css'] = [
    '#type' => 'textfield',
    '#title' => t('Bootstrap CDN CSS'),
    '#default_value' => theme_get_setting('kalatheme_bootstrap_cdn_css'),
    '#description' => t('You can override this CSS to use a different version if need be'),
    '#states' => [
      'visible' => [
        'select[name="framework"]' => ['value' => 'bootstrap'],
        'input[name="bootstrap_enable_cdn"]' => ['checked' => TRUE],
      ],
    ],
  ];

  // Bootstrap CDN JS URL.
  $form['framework_config']['bootstrap_cdn']['bootstrap_cdn_js'] = [
    '#type' => 'textfield',
    '#title' => t('Bootstrap CDN JS'),
    '#default_value' => theme_get_setting('kalatheme_bootstrap_cdn_js'),
    '#description' => t('You can override this JS to use a different version if need be'),
    '#states' => [
      'visible' => [
        'select[name="framework"]' => ['value' => 'bootstrap'],
        'input[name="bootstrap_enable_cdn"]' => ['checked' => TRUE],
      ],
    ],
  ];


  $form['#submit'][] = '_kalatheme_system_theme_settings_submit';
}

/**
 * Submit Handler for kalatheme_form_system_theme_settings_alter().
 */
function _kalatheme_system_theme_settings_submit(&$form, FormStateInterface $form_state) {
  // Clean up the values
  $form_state->cleanValues();

  // Load up Config Factory for Kalatheme, don;t add any if not there.
  $config = \Drupal::configFactory()->getEditable('kalatheme.settings', FALSE);

  // Go through each value and set it to appropriate value.
  foreach ($form_state->getValues() as $name => $value) {
    $config->set('kalatheme_' . $name, $value);
  }

  // Save this up.
  $config->save();
}
