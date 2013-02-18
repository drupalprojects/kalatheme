<?php
/**
 * @file
 * Template for Kalatheme Ward.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display ward clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>

  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="ward-container ward-header clearfix panel-panel">
        <div class="ward-container-inner ward-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>

  <section class='section'>
    <div class='container'>
      <div class='row-fluid'>
        <div class="ward-container ward-column-content clearfix">
          <div class="ward-column-content-region ward-content panel-panel span9">
            <div class="ward-column-content-region-inner ward-content-inner panel-panel-inner">
              <?php print $content['contentmain']; ?>
            </div>
          </div>
          <div class="ward-column-content-region ward-sidebar panel-panel span3">
            <div class="ward-column-content-region-inner ward-sidebar-inner panel-panel-inner">
              <?php print $content['sidebar']; ?>
            </div>
          </div>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>
      <div class="ward-container ward-column-content ward-column-content-row1 clearfix row-fluid">
        <div class="ward-column-content-region ward-column ward-column1 panel-panel span4">
          <div class="ward-column-content-region-inner ward-column-inner ward-column1-inner panel-panel-inner">
            <?php print $content['footer1']; ?>
          </div>
        </div>
        <div class="ward-column-content-region ward-column ward-column2 panel-panel span4">
          <div class="ward-column-content-region-inner ward-column-inner ward-column2-inner panel-panel-inner">
            <?php print $content['footer2']; ?>
          </div>
        </div>
        <div class="ward-column-content-region ward-column ward-column3 panel-panel span4">
          <div class="ward-column-content-region-inner ward-column-inner ward-column3-inner panel-panel-inner">
            <?php print $content['footer3']; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
  
</div><!-- /.ward -->
