<?php
/**
 * @file
 * Kalatheme's subtheme config form stuff
 */

/**
 * Batch callback: Copies project to its proper place when authorized to do so.
 *
 * @param string $project
 *   The canonical short name of the project being installed.
 * @param string $updater_name
 *   The name of the Updater class to use for installing this project.
 * @param string $local_url
 *   The URL to the locally installed temp directory where the project has
 *   already been downloaded and extracted into.
 * @param FileTransfer $filetransfer
 *   The FileTransfer object to use for performing this operation.
 * @param array $context
 *   Reference to an array used for Batch API storage.
 */
function kalatheme_authorize_batch_copy_project($project, $updater_name, $local_url, $filetransfer, &$context) {
  module_load_include('inc', 'update', 'update.authorize');
  // Initialize some variables in the Batch API $context array.
  if (!isset($context['results']['log'])) {
    $context['results']['log'] = array();
  }
  if (!isset($context['results']['log'][$project])) {
    $context['results']['log'][$project] = array();
  }

  if (!isset($context['results']['tasks'])) {
    $context['results']['tasks'] = array();
  }

  // The batch API uses a session, and since all the arguments are serialized
  // and unserialized between requests, although the FileTransfer object itself
  // will be reconstructed, the connection pointer itself will be lost. However,
  // the FileTransfer object will still have the connection variable, even
  // though the connection itself is now gone. So, although it's ugly, we have
  // to unset the connection variable at this point so that the FileTransfer
  // object will re-initiate the actual connection.
  unset($filetransfer->connection);

  if (!empty($context['results']['log'][$project]['#abort'])) {
    $context['finished'] = 1;
    return;
  }
  if ($updater_name == 'KalathemeUpdater') {
    require_once dirname(__FILE__) . '/kalatheme.updater.inc';
  }
  $updater = new $updater_name($local_url);

  try {
    if ($updater->isInstalled()) {
      // This is an update.
      $tasks = $updater->update($filetransfer);
    }
    else {
      $tasks = $updater->install($filetransfer);
    }
  }
  catch (UpdaterException $e) {
    _update_batch_create_message($context['results']['log'][$project], t('Error installing / updating'), FALSE);
    _update_batch_create_message($context['results']['log'][$project], $e->getMessage(), FALSE);
    $context['results']['log'][$project]['#abort'] = TRUE;
    return;
  }

  _update_batch_create_message($context['results']['log'][$project], t('Installed %project_name successfully', array('%project_name' => $project)));
  if (!empty($tasks)) {
    $context['results']['tasks'] += $tasks;
  }

  // This particular operation is now complete, even though the batch might
  // have other operations to perform.
  $context['finished'] = 1;
}
