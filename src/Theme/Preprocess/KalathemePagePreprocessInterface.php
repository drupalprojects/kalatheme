<?php
/**
 * @file
 * Contains \Drupal\kalatheme\Theme\Preprocess\KalathemePreprocessInterface.
 */

namespace Drupal\kalatheme\Theme\Preprocess;

/**
 * Contains the interface for some of the page type preprocessors.
 *
 * Not all are in here and are defined in KalthemeBase for specific needs.
 *
 * @see \Drupal\kalatheme\Theme\KalathemeBase
 *
 * Gives the structure to manipulate in the frameworks.
 */
interface KalathemePagePreprocessInterface {

  /**
   * Return altered variables for HTML document templates.
   *
   * @return array
   *   An associative array of the same format as returned by
   *   template_preprocess_html().
   *
   * @see template_preprocess_html
   */
  public function preprocessHtml(array &$variables);
}
