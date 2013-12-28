<?php
/**
 * @file
 * Template for Panopoly Brenham Flipped.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display brenham-flipped clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  <section class="section alt" id="promo">
    <div class="container">
      <div class="row">
        <div class="col-md-12 brenham-flipped-promo">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-8 brenham-flipped-main-content">
          <?php print $content['contentmain']; ?>
        </div>
        <div class="col-md-4 brenham-flipped-main-side">
          <?php print $content['sidebar']; ?>
        </div>
      </div>
    </div>
  </section>
</div><!-- /.brenham-flipped -->
