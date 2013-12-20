<?php
/**
 * @file
 * Template for Panopoly Boxton.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display boxton clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  <section class='section'>
    <div class='container'>
      <div class="row-fluid">
        <div class="boxton-container boxton-content boxton-content-region panel-panel span12">
          <?php print $content['contentmain']; ?>
        </div>
      </div>
    </div><!-- /.container -->
  </section><!--  /.section -->
</div><!-- /.boxton -->


