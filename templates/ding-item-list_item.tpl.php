<?php
/**
 * @file
 *
 * Template file for single item list item.
 */
?>

<div class="ding-item-list-item">
  <div class="item-cover">
    <img src="<?php print $cover; ?>" alt="" />
  </div>
  <div class="item-details">
    <div class="item-title"><?php print $title; ?></div>
    <div class="item-author"><?php print t('By');?> <?php print $author; ?> (<?php print $year;?>)</div>
    <div class="item-rating">
      <div class="rating-value-<?php print $rating; ?>"><?php print $rating; ?></div>
      <span class="rating-count">(<?php print $rating_count; ?>)</span></div>
    <div class="item-reviews">(<?php print $review_count;?>) <?php print t('reviews'); ?></div>
    <div class="item-loan"><?php print $loan_form; ?></div>
  </div>
</div>
