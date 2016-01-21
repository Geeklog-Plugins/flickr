<?php

/* Reminder: always indent with 4 spaces (no tabs). */
// +---------------------------------------------------------------------------+
// | Flickr Plugin 1.0                                                         |
// +---------------------------------------------------------------------------+
// | english.php                                                               |
// |                                                                           |
// | English language file                                                     |
// +---------------------------------------------------------------------------+
// | Copyright (C) 2009 by the following authors:                              |
// |                                                                           |
// | Authors: ::Ben                                                            |
// +---------------------------------------------------------------------------+
// | Created with the Geeklog Plugin Toolkit.                                  |
// +---------------------------------------------------------------------------+
// |                                                                           |
// | This program is free software; you can redistribute it and/or             |
// | modify it under the terms of the GNU General Public License               |
// | as published by the Free Software Foundation; either version 2            |
// | of the License, or (at your option) any later version.                    |
// |                                                                           |
// | This program is distributed in the hope that it will be useful,           |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of            |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
// | GNU General Public License for more details.                              |
// |                                                                           |
// | You should have received a copy of the GNU General Public License         |
// | along with this program; if not, write to the Free Software Foundation,   |
// | Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           |
// |                                                                           |
// +---------------------------------------------------------------------------+

/**
* @package Flickr
*/

/**
* Import Geeklog plugin messages for reuse
*
* @global array $LANG32
*/
global $LANG32;

// +---------------------------------------------------------------------------+
// | Array Format:                                                             |
// | $LANGXX[YY]:  $LANG - variable name                                       |
// |               XX    - specific array name                                 |
// |               YY    - phrase id or number                                 |
// +---------------------------------------------------------------------------+

$LANG_FLICKR_1 = array(
    'plugin_name'                   => 'Flickr',
	'plugin_doc'                    => 'Documentation is also',
	'online'                        => 'online',
	'plugin_conf'                   => 'The Flickr plugin configuration is',
	'autotag_desc_flickr'           => '[flickr: id Options Title] - Displays picture where id is the photo\'s id. The options are: <li>align: attribute accepts left and right as alignments,</li> <li>size: attribute accepts square, thumbnail (which is the default when no size is given), small, and medium.</li> If a Title is specified the picture will replace by this Title.',
	
	'autotag_desc_flickr_gallery'   => '[flickr_gallery: X Options Caption] X is the thumbnail limit, maximum is 500. Options are:<li>user_id: The NSID of the user who\'s photo to search.</li><li>tags: A comma-delimited list of tags.</li> <li>min_upload_date: Minimum upload date. The date should be in the form of a unix timestamp.</li> <li>max_upload_date: Maximum upload date.</li><li>min_taken_date: Minimum taken date.</li> <li>max_taken_date: Maximum taken date.</li> <li>sort : The order in which to sort returned photos. The possible values are: date-posted-asc, date-posted-desc, date-taken-asc, date-taken-desc, interestingness-desc, interestingness-asc, and relevance.</li> <li>bbox: A comma-delimited list of 4 values defining the Bounding Box of the area that will be searched. Longitude has a range of -180 to 180, latitude of -90 to 90. Defaults to -180, -90, 180, 90 if not specified.</li> <li>group_id: The id of a group who\'s pool to search.</li> <li>lat: A valid latitude, in decimal format, for doing radial geo queries.</li> <li>lon: A valid longitude, in decimal format, for doing radial geo queries.</li> <li>radius: A valid radius greater than zero and less than 20 miles (or 32 kilometers).</li> <li>size: s for \'Square\', t for \'Thumbnail (100px)\', m for \'Thumbnail (240px)\', - for \'Thumbnail (500px)\', z for \'Thumbnail (640px)\', b for \'Thumbnail(1024px)\', and o for \'Original\'.</li>',
	
	'autotag_desc_flickr_slideshow' => '[flickr_slideshow: id Options] The id parameter is the flickr user ID. Options are: <li>set_id: Optional set ID for this user.</li><li>width: in pixels or %</li><li>height: in pixels</li>',
);

// Localization of the Admin Configuration UI
$LANG_configsections['flickr'] = array(
    'label' => 'flickr',
    'title' => 'flickr Configuration'
);

$LANG_confignames['flickr'] = array(
    'flickrloginrequired' => 'flickr Login Required?',
    'hideflickrmenu'      => 'Hide flickr Menu Entry?',
	'replacefrontpage'    => 'Replace front page',
	'blankpage'           => 'Blank page',
	'profile_hook'        => 'Add flickr to profile',
	'use_cache'           => 'Use cache',
	'cache_type'          => 'Cache type',
	'cache_expiration'    => 'Cache expiration',
	'cache_table_name'    => 'Cache table name',
	'cache_fs_name'       => 'Path to cache files',
	'use_CND'             => 'Use CND for jQuery library',
	'mini_gallery_format' => 'Mini gallery pictures format',
	'flickr_api_key'      => 'Your flickr api key. Get one from http://www.flickr.com/services/api/',

);

$LANG_configsubgroups['flickr'] = array(
    'sg_0' => 'Main'
);

$LANG_fs['flickr'] = array(
    'fs_01' => 'Cache',
    'fs_02' => 'jQuery',
	'fs_03' => 'Theme',
	'fs_04' => 'Flickr'
);

// Note: entries 0, 1, and 12 are the same as in $LANG_configselects['Core']
$LANG_configselects['flickr'] = array(
    0  => array('True' => 1, 'False' => 0),
    1  => array('True' => TRUE, 'False' => FALSE),
	21 => array('database' => 'db', 'filesystem' => 'fs'),
    22 => array('Square' => '_s', 'Thumbnail (100px)' => '_t', 'Thumbnail (240px)' => '_m', 'Thumbnail (500px)' => '_-', 'Thumbnail (640px)' => '_z', 'Thumbnail(1024px)' => '_b', 'Original' => '_o')
);

// Messages for the plugin upgrade
$PLG_flickr_MESSAGE3002 = $LANG32[9]; // "requires a newer version of Geeklog"

?>
