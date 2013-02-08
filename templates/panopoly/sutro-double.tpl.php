<?php
/**
 * @file
 * Template for Panopoly Sutro Double.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display sutro-double clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="sutro-double-container sutro-double-header clearfix panel-panel ">
        <div class="sutro-double-container-inner sutro-double-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>

  <section class='section'>
    <div class='container'>  
      <div class="sutro-double-container sutro-double-column-content sutro-double-first-column-content clearfix row-fluid">
        <div class="sutro-double-column-content-region sutro-double-column1 sutro-double-column panel-panel span6">
          <div class="sutro-double-column-content-region-inner sutro-double-column1-inner sutro-double-column-inner panel-panel-inner">
            <?php print $content['column1']; ?>
          </div>
        </div>
        <div class="sutro-double-column-content-region sutro-double-column2 sutro-double-column panel-panel span6">
          <div class="sutro-double-column-content-region-inner sutro-double-column2-inner sutro-double-column-inner panel-panel-inner">
            <?php print $content['column2']; ?>
          </div>
        </div>
      </div>
      
      <div class="sutro-double-container sutro-double-middle clearfix panel-panel span12">
        <div class="sutro-double-container-inner sutro-double-middle-inner panel-panel-inner">
          <?php print $content['middle']; ?>
        </div>
      </div>
      
      <div class="sutro-double-container sutro-double-column-content sutro-double-second-column-content clearfix row-fluid">
        <div class="sutro-double-column-content-region sutro-double-column1 sutro-double-column panel-panel span6">
          <div class="sutro-double-column-content-region-inner sutro-double-column1-inner sutro-double-column-inner panel-panel-inner">
            <?php print $content['secondcolumn1']; ?>
          </div>
        </div>
        <div class="sutro-double-column-content-region sutro-double-column2 sutro-double-column panel-panel span6">
          <div class="sutro-double-column-content-region-inner sutro-double-column2-inner sutro-double-column-inner panel-panel-inner">
            <?php print $content['secondcolumn2']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>  
      <div class="sutro-double-container sutro-double-footer clearfix panel-panel">
        <div class="sutro-double-container-inner sutro-double-footer-inner panel-panel-inner">
          <?php print $content['footer']; ?>
        </div>
      </div>
    </div>
  </footer>
    
</div><!-- /.sutro-double -->
