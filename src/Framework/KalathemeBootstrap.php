<?php

/**
 * @file
 * Contains \Drupal\kalatheme\Framework\KalathemeBootstrap.
 */

namespace Drupal\kalatheme\Framework;

use \Drupal\kalatheme\Theme\KalathemeBase;
use \Drupal\kalatheme\Theme\Preprocess\KalathemePagePreprocessInterface;
use \Drupal\kalatheme\Theme\Preprocess\KalathemeMenuPreprocessInterface;

/**
 * Defines base class to be used with all Framework Classes.
 */
class KalathemeBootstrap extends KalathemeBase implements KalathemePagePreprocessInterface, KalathemeMenuPreprocessInterface {

  /**
   * {@inheritdoc}
   */
  public function preprocessHtml(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessPage(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuMain(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuSecondary(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuTree(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuLink(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuLocalTasks(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuLocalTask(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuLocalActions(array &$variables) {}

  /**
   * {@inheritdoc}
   */
  public function preprocessMenuLocalAction(array &$variables) {}
}
