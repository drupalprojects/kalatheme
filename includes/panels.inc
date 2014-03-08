<?php
/**
 * @file
 * Theme and hook functions for panels things.
 */

/*
 * Implements hook_panels_default_style_render_region().
 *
 * Some magic from @malberts with inspiration from Omega
 */
function kalatheme_panels_default_style_render_region(&$variables) {
  // Remove .panels-separator.
  return implode('', $variables['panes']);
}

/**
 * Implements hook_preprocess_panels_add_content_link().
 */
function kalatheme_preprocess_panels_add_content_link(&$vars) {
  $vars['text_button'] = ctools_ajax_text_button($vars['title'], $vars['url'], $vars['description'], 'panels-modal-add-config btn btn-default');
}

