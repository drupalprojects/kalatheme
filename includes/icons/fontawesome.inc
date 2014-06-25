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
  $icons = &drupal_static(__FUNCTION__);
  // If not set get the drupal cache.
  if (!isset($icons)) {
    // Check the cache and set it.
    if ($cache = cache_get('kalatheme_font_awesome_icons')) {
      $icons = $cache->data;
    }
    // If not go ahead and build the icon set.
    else {
      $icons = array();
      if (file_exists(__DIR__ . '/fontawesome.json')) {
        $file = file_get_contents(__DIR__ . '/fontawesome.json');
        $raw_icons = json_decode($file, TRUE);
        foreach ($raw_icons['icons'] as $delta => $icon) {
          $icons[$icon['id']] = $icon['name'];
        }
      }
      // Set the cache to avoid reading the file again.
      cache_set('kalatheme_font_awesome_icons', $icons, 'cache');
    }
  }
  return $icons;
}
