<?php

/* Reminder: always indent with 4 spaces (no tabs). */
// +---------------------------------------------------------------------------+
// | flickr plugin 1.0.0                                                         |
// +---------------------------------------------------------------------------+
// | install_defaults.php                                                      |
// |                                                                           |
// | Initial Installation Defaults used when loading the online configuration  |
// | records. These settings are only used during the initial installation     |
// | and not referenced any more once the plugin is installed.                 |
// +---------------------------------------------------------------------------+
// | Copyright (C) 2008 by the following authors:                              |
// |                                                                           |
// | Authors: Dirk Haun        - dirk AT haun-online DOT de                    |
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
//

if (strpos(strtolower($_SERVER['PHP_SELF']), 'install_defaults.php') !== false) {
    die('This file can not be used on its own!');
}

/*
 * flickr default settings
 *
 * Initial Installation Defaults used when loading the online configuration
 * records. These settings are only used during the initial installation
 * and not referenced any more once the plugin is installed
 *
 */
 
// Note FL is for flickr
 
global $_DB_table_prefix, $_FL_DEFAULT;
$_FL_DEFAULT = array();

// If set to true, use one of the available cache options (see below).
// Note that your site may slow down significantly if you do NOT use caching.
$_FL_DEFAULT['use_cache'] = true;

// Type of caching to use when enables (see above):
// 'db': use database to cache - need DB::PEAR package
// 'fs': use filesystem cache - needs a writable directory
$_FL_DEFAULT['cache_type'] = 'fs'; // 'db' (database) or 'fs' (filesystem)

// how long to cache entries
$_FL_DEFAULT['cache_expiration'] = 600; // in seconds

// path to store cache files in when 'fs' caching is enabled
$_FL_DEFAULT['cache_fs_name'] = $_CONF['path_data'];



$_FL_DEFAULT['mini_gallery_format'] = '_s'; // '_s' (square) or '_t' (thumnail)

/**
* Initialize flickr plugin configuration
*
* Creates the database entries for the configuation if they don't already
* exist. 
*
* @return   boolean     true: success; false: an error occurred
*
*/
function plugin_initconfig_flickr()
{
    global $_CONF, $_FL_DEFAULT;

    $c = config::get_instance();
    if (!$c->group_exists('flickr')) {

        //This is main subgroup #0
		$c->add('sg_0', NULL, 'subgroup', 0, 0, NULL, 0, true, 'flickr');
		
		//This is fieldset #1  in subgroup #0   
		$c->add('fs_01', NULL, 'fieldset', 0, 0, NULL, 0, true, 'flickr');
        $c->add('use_cache', $_FL_DEFAULT['use_cache'],
                'select', 0, 0, 0, 10, true, 'flickr');
        $c->add('cache_type', $_FL_DEFAULT['cache_type'], 'select',
		        0, 0, 21, 20, true, 'flickr');
		$c->add('cache_expiration', $_FL_DEFAULT['cache_expiration'], 'text',
                0, 0, 0, 30, true, 'flickr');	
		$c->add('cache_fs_name', $_FL_DEFAULT['cache_fs_name'], 'text',
                0, 0, 0, 50, true, 'flickr');
		
		//This is fieldset #3  in subgroup #0   
		$c->add('fs_03', NULL, 'fieldset', 0, 2, NULL, 0, true, 'flickr');
        $c->add('mini_gallery_format', $_FL_DEFAULT['mini_gallery_format'],
                'select', 0, 2, 22, 70, true, 'flickr');

		//This is fieldset #4  in subgroup #0   
		$c->add('fs_04', NULL, 'fieldset', 0, 3, NULL, 0, true, 'flickr');
		$c->add('flickr_api_key', '', 'text', 0, 3, 0, 80, true, 'flickr');				
    }

    return true;
}

?>