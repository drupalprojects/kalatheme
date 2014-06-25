<?php
/**
 * @file
 * Kalatheme's copy of all the glyphicon names
 */

function _kalatheme_fontawesome_icons() {

  $icons = &drupal_static(__FUNCTION__);
  // if not set get the drupal cache
  if (!isset($icons)){

    //check the cache and set it
    if($cache = cache_get('kalatheme_font_awesome_icons')) {
      $icons = $cache->data;
    }
    // if not go ahead and build the icon set.
    else{
      $file = file_get_contents(__DIR__ . '/fontawesome.json');
      $raw_icons = json_decode($file, TRUE);
      $icons = array();
      foreach ($raw_icons['icons']as $delta => $icon) {
        $icons[$icon['id']] = $icon['name'];
      }
      cache_set('kalatheme_font_awesome_icons', $icons, 'cache');
    }
  }
  return $icons;
}
