<?php

/**
 * @file
 * Contains \Drupal\kalatheme\Framework\KalathemeBootstrap.
 */

namespace Drupal\kalatheme\Framework;

use \Drupal\kalatheme\Theme\KalathemeBase;
use \Drupal\kalatheme\Theme\Preprocess\KalathemePagePreprocessInterface;

/**
 * Defines base class to be used with all Framework Classes.
 */
class KalathemeBootstrap extends KalathemeBase implements KalathemePagePreprocessInterface {

  /**
   * {@inheritdoc}
   */
  public function preprocessHtml(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessPage(array &$variables) {

  }
}
