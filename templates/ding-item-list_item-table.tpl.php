<?php
/**
 * @file
 * Template file for single item list item.
 */
$alt = check_plain($title . ' ' . $author);
?>
<table class="ding-item-list-item">
  <tbody>
  <tr>
    <td class="item-cover">
      <a href="/ting/object/<?php print $faust; ?>">
        <img src="<?php print $cover; ?>" alt="<?php echo $alt; ?>"/>
      </a>
    </td>
    <td class="item-details">
      <table>
        <tbody>
        <tr>
          <td class="item-title"><a
               href="/ting/object/<?php print $faust; ?>"><?php print $title; ?></a>
          </td>
        </tr>
        <tr>
          <td class="item-author"><?php if (!empty($author)) {
              print t('By @author', array('@author' => $author));
            } ?> (<?php print $year; ?>)
          </td>
        </tr>
        <?php if (isset($has_rating)): ?>
          <tr>
            <td class="item-rating">
              <div
                 class="rating-value-<?php print $rating; ?>"><?php print $rating; ?></div>
              <span class="rating-count">(<?php print $rating_count; ?>)</span>
            </td>
            <div class="item-reviews">(<?php print $review_count; ?>
              ) <?php print t('reviews'); ?></div>
          </tr>
        <?php endif; ?>
        <?php if (!empty($loan_form)): ?>
          <tr>
            <td class="item-loan"><?php print $loan_form; ?></td>
          </tr>
        <?php endif; ?>
        <?php if (!empty($description)): ?>
          <tr>
            <td class="item-description"><?php print $description; ?></td>
          </tr>
        <?php endif; ?>
        </tbody>
      </table>
    </td>
  </tr>
  </tbody>
</table>
