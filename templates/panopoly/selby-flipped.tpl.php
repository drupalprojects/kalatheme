<?php
/**
 * @file
 * Template for Panopoly selby-flipped-flipped Flipped.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display selby-flipped-flipped-flipped clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  <section class="section">
    <div class="container">
      <div class="row">
        <div class="col-md-8 selby-flipped-column-content-region-area">
          <div class="row">
            <div class="col-md-12 selby-flipped-column-content-region-area">
              <?php print $content['contentheader']; ?>
            </div>
          </div>
          <div class="row">
            <div class="col-md-6 selby-flipped-column-content-region-1">
              <?php print $content['contentcolumn1']; ?>
            </div>
            <div class="col-md-6 selby-flipped-column-content-region-2">
              <?php print $content['contentcolumn2']; ?>
            </div>
          </div><!-- /.selby-flipped-content-container row-->
          <div class="row">
            <div class="col-md-12 selby-flipped-content-footer-area">
              <?php print $content['contentfooter']; ?>
            </div>
          </div>
        </div><!-- /.selby-flipped-content-container -->
        <div class="col-md-4 selby-flipped-sidebar-main-area">
          <div class="row">
            <div class="col-md-12 selby-flipped-sidebar-area">
              <?php print $content['sidebar']; ?>
            </div>
          </div>
        </div> <!-- /.selby-flipped-sidebar col-md-4-->
      </div><!-- /.selby-flipped-content-container row-->
    </div>
  </section>
</div><!-- /.selby-flipped-flipped-flipped -->
