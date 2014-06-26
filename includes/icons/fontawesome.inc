<?php
/**
 * @file
 * includes/icons/fontawesome.inc
 * Returns the fontawesome icons.
 */

/**
 * Helper function to return icons for font awesome.
 */
function _kalatheme_fontawesome_icons() {
  $icons = array();
  // Load the font awesome json file.
  if (file_exists(__DIR__ . '/fontawesome.json')) {
    $file = file_get_contents(__DIR__ . '/fontawesome.json');
    $raw_icons = json_decode($file, TRUE);
    foreach ($raw_icons['icons'] as $delta => $icon) {
      $icons[$icon['id']] = $icon['name'];
    }
  }
  return $icons;
}
