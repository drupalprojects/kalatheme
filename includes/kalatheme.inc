<?php

/**
 * @file
 * Kalatheme's helper functions
 */

/**
 * Message displayed when user does not have Panopoly installed
 */
define('KALATHEME_PANOPOLY_ERROR', t('In order to use Kalatheme, you must ' .
  'have the Panopoly distribution installed. Please check the README for ' .
  'installation instructions.'));
/**
 * Message displayed when user does not have Bootstrap available
 */
define('KALATHEME_BOOTSTRAP_ERROR', t('In order to use Kalatheme, you must ' .
  'have the Twitter Bootstrap library available. Please check the README for ' .
  'installation instructions.'));

/**
 * Loads all dependencies necessary for Kalatheme to operate
 *
 * @return string
 *   Name of the function for kalatheme_status_messages to call
 */
function _kalatheme_load_dependencies() {
  //If we've already loaded dependencies, just return the correct function to
  //call for status messages
  $status_function = &drupal_static(__FUNCTION__);
  if (isset($status_function)) {
    return $status_function;
  }
  // Add what we need from Bootstrap and make sure Panopoly is installed
  $all_loaded = FALSE;
  if (module_exists('panopoly_core')) {
    if (($library = libraries_detect('bootstrap')) && !empty($library['installed'])) {
      $bootstrap_path = DRUPAL_ROOT . '/' . $library['library path'];
      $variant = NULL;
      $has_minified_css = file_exists($bootstrap_path . '/css/bootstrap.min.css');
      $has_minified_js = file_exists($bootstrap_path . '/js/bootstrap.min.js');
      if ($has_minified_css && $has_minified_js) {
        $variant = 'minified';
      }
      libraries_load('bootstrap', $variant);
      $all_loaded = TRUE;
    }
    else {
      // Something went wrong. :(
      drupal_set_message(KALATHEME_BOOTSTRAP_ERROR, 'error');
    }
  }
  else {
    drupal_set_message(KALATHEME_PANOPOLY_ERROR, 'error');
  }
  //Return the correct status message function depending on whether we were able
  //to load all of our dependencies
  if (!$all_loaded) {
    $status_function = 'theme_status_messages';
  }
  else {
    $status_function = '_kalatheme_status_messages';
  }
  return $status_function;
}
