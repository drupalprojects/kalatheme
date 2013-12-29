<?php

/**
 * @file
 * Kalatheme's subtheme setup stuff
 */

/**
 * Form for kalatheme setup
 */
function kalatheme_setup_form($form, &$form_state) {

  $form['help_text'] = array(
    '#prefix' => '<p>',
    '#markup' => t('Here you can easily setup a Kalatheme subtheme with the default or a custom Bootstrap library. For more information please consult our <a href="@setup_url">Start Up Guide</a> or just give it a whirl! The following file extensions are supported: %extensions.', array(
      '@setup_url' => 'http://drupal.org/project/kalatheme',
      '%extensions' => archiver_get_extensions(),
    )),
    '#suffix' => '</p>',
  );

  $form['subtheme_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name your theme'),
    '#description' => t('Use only lower case letters and underscores. For example: mytheme, mega_awesome_theme'),
  );

  $form['bootstrap_upload'] = array(
    '#type' => 'file',
    '#title' => t('Bootstrap library'),
    '#description' => t('Upload any valid bootstrap library here to complete your install. For example: %filename from your local computer', array('%filename' => 'name.tar.gz')),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set me up!'),
  );
  return $form;
}

/**
 * Form validation handler for kalatheme_setup_form().
 *
 * @see kalatheme_setup_form_submit()
 */
function kalatheme_setup_form_validate($form, &$form_state) {
  // Validate machine name
  $regex = '/^[A-Za-z][A-Za-z\d_]*$/';
  if (!preg_match($regex, $form_state['values']['subtheme_name'])) {
    form_set_error('subtheme_name', t('Please enter a valid machine name'));
  }
  // Validate file exists
  if (empty($_FILES['files']['name']['bootstrap_upload'])) {
    form_set_error('bootstrap_upload', t('You must either provide a URL or upload an archive file to install.'));
  }
}

/**
 * Form submission handler for kalatheme_setup_form().
 *
 * Uploads the file attached to the form, then attempts to extract the archive
 * into a temporary location and verify it. Verifies the archive is a valid Bootsrap
 * library and if possible installs the library into the libraries API directory. If not
 * saves it for output to the user.
 *
 * Then builds a subtheme using the name provided and tries to install it. If it cannot it
 * grabs the bootstrap library and outputs both as an archive for the user to install.
 *
 * @see kalatheme_setup_form_validate()
 */
function kalatheme_setup_form_submit($form, &$form_state) {
  if ($_FILES['files']['name']['bootstrap_upload']) {
    $validators = array('file_validate_extensions' => array(archiver_get_extensions()));
    $field = 'bootstrap_upload';
    if (!($finfo = file_save_upload($field, $validators, NULL, FILE_EXISTS_REPLACE))) {
      // Failed to upload the file. file_save_upload() calls form_set_error() on
      // failure.
      return;
    }
    $local_cache = $finfo->uri;
  }

  $directory = _kalatheme_extract_directory();
  try {
    $archive = kalatheme_archive_extract($local_cache, $directory);
  }
  catch (Exception $e) {
    form_set_error($field, $e->getMessage());
    return;
  }

  $files = $archive->listContents();
  if (!$files) {
    form_set_error($field, t('Provided archive contains no files.'));
    return;
  }
}

/**
 * Copy of update_manager_archive_extract() Just in case update module is not
 * enabled. Unpacks a downloaded archive file.
 *
 * @param string $file
 *   The filename of the archive you wish to extract.
 * @param string $directory
 *   The directory you wish to extract the archive into.
 *
 * @return Archiver
 *   The Archiver object used to extract the archive.
 *
 * @throws Exception
 *
 * @see update_manager_archive_extract()
 */
function kalatheme_archive_extract($file, $directory) {
  $archiver = archiver_get_archiver($file);
  if (!$archiver) {
    throw new Exception(t('Cannot extract %file, not a valid archive.', array ('%file' => $file)));
  }

  // Remove the directory if it exists, otherwise it might contain a mixture of
  // old files mixed with the new files (e.g. in cases where files were removed
  // from a later release).
  $files = $archiver->listContents();

  // Unfortunately, we can only use the directory name to determine the project
  // name. Some archivers list the first file as the directory (i.e., MODULE/)
  // and others list an actual file (i.e., MODULE/README.TXT).
  $project = strtok($files[0], '/\\');

  $extract_location = $directory . '/' . $project;
  if (file_exists($extract_location)) {
    file_unmanaged_delete_recursive($extract_location);
  }

  $archiver->extract($directory);
  return $archiver;
}

/**
 * Copy of _update_manager_extract_directory() Just in case update module is
 * not enabled.
 *
 * @see _update_manager_extract_directory()
 */
function _kalatheme_extract_directory($create = TRUE) {
  $directory = &drupal_static(__FUNCTION__, '');
  if (empty($directory)) {
    $directory = 'temporary://kalatheme-extraction-' . _kalatheme_unique_identifier();
    if ($create && !file_exists($directory)) {
      mkdir($directory);
    }
  }
  return $directory;
}

/**
 * Returns a short unique identifier for this Drupal installation.
 *
 * @return
 *   An eight character string uniquely identifying this Drupal installation.
 */
function _kalatheme_unique_identifier() {
  $id = &drupal_static(__FUNCTION__, '');
  if (empty($id)) {
    $id = substr(hash('sha256', drupal_get_hash_salt()), 0, 8);
  }
  return $id;
}
