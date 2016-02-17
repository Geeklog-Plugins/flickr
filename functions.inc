<?php

/* Reminder: always indent with 4 spaces (no tabs). */
// +---------------------------------------------------------------------------+
// | Flickr Plugin 1.2.2                                                       |
// +---------------------------------------------------------------------------+
// | functions.inc                                                             |
// |                                                                           |
// | This file does two things: 1) it implements the necessary Geeklog Plugin  |
// | API methods and 2) implements all the common code needed by this plugin.  |
// +---------------------------------------------------------------------------+
// | Copyright (C) 2014 by the following authors:                              |
// |                                                                           |
// | Authors: Dirk Haun        - dirk AT haun-online DOT de                    |
// |          ::Ben              http://geeklog.fr                             |
// |          Felipe Skroski     http://www.felipesk.com                       |
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

if (strpos(strtolower($_SERVER['PHP_SELF']), 'functions.inc') !== false) {
    die ('This file can not be used on its own.');
}

/**
 * Language file include
 */
$plugin_path = $_CONF['path'] . 'plugins/flickr/';
$langfile = $plugin_path . 'language/' . $_CONF['language'] . '.php';

if (file_exists($langfile)) {
    require_once $langfile;
} else {
    require_once $plugin_path . 'language/english.php';
}
/**
 * Flickr plugin table(s)
 */
$_TABLES['flickr_cache'] = $_DB_table_prefix . 'flickr_cache';

/**
 * Load our config
 */
require_once $_CONF['path_system'].'classes/config.class.php';
$c = config::get_instance();
$_FL_CONF = $c->get_config('flickr');

// This is the Flickr API key for the Geeklog Flickr plugin.
// Should you create any derived work from this plugin, please get your own
// API key (it's free) from http://www.flickr.com/services/api/
define ('FLICKR_API_KEY', $_FL_CONF['flickr_api_key']);


// +---------------------------------------------------------------------------+
// | Geeklog Plugin API Implementation                                         |
// +---------------------------------------------------------------------------+


/**
* Implements autotags
*
* @param    string  $op         operation to perform
* @param    string  $content    item (e.g. story text), including the autotag
* @param    array   $autotag    parameters used in the autotag
* @param    mixed               tag names (for $op='tagname') or formatted content
*
*/

function plugin_autotags_flickr ($op, $content = '', $autotag = '')
{
    global $_CONF, $_FL_CONF, $_TABLES, $LANG_FLICKR_1, $_DB_host, $_DB_name, $_DB_user, $_DB_pass, $_SCRIPTS;

    if ($op == 'tagname') {

        return array('flickr', 'flickr_gallery', 'flickr_slideshow');

    } else if ($op == 'description') {
        return array (
            'flickr'           => $LANG_FLICKR_1['autotag_desc_flickr'],
			'flickr_gallery'   => $LANG_FLICKR_1['autotag_desc_flickr_gallery'],
			'flickr_slideshow' => $LANG_FLICKR_1['autotag_desc_flickr_slideshow'],
            );
	} else if ($op == 'parse') {

        require_once ($_CONF['path'] . 'plugins/flickr/include/phpFlickr.php');
        
        $_SCRIPTS->setJavaScriptLibrary('jquery');
	
        $css = '/flickr/css/flickr120.css';
        $_SCRIPTS->setCSSFile('flickr.css', $css, true);
        
        $str = LB . '    var flickr_api_key = "'. $_FL_CONF['flickr_api_key'] .'";' . LB;
        $str .= '    var flickr_mini_gallery_img_format = "' . $_FL_CONF['mini_gallery_format'] . '";' . LB;
        /*
        $str .= '    jQuery(document).ready(function(){' . LB;
        $str .= '    initialize_flickr()	;' . LB;
        $str .= '    });' . LB;
        */
        $_SCRIPTS->setJavaScript($str, true, true);
            
        $strext = '/flickr/js/miniflickr-0.4.js';
        $_SCRIPTS->setJavaScriptFile('flickr.miniflickr', $strext, false);
		
		static $flickr;

        if (!isset ($flickr)) {
            $flickr = new phpFlickr (FLICKR_API_KEY, NULL, false);

            if ($_FL_CONF['use_cache']) {
                if ($_FL_CONF['cache_type'] == 'db') {
                    $flickr->enableCache ('db',
                            "mysql://$_DB_user:$_DB_pass@$_DB_host/$_DB_name",
                            $_FL_CONF['cache_expiration'],
                            $_TABLES['flickr_cache']);
                } else {
                    // phpFlickr prefers its paths without a trailing slash
                    if (substr ($_FL_CONF['cache_fs_name'], -1) == '/') {
                        $_FL_CONF['cache_fs_name'] = substr ($_FL_CONF['cache_fs_name'], 0, -1);
                    }
                    $flickr->enableCache ('fs', $_FL_CONF['cache_fs_name'],
                                          $_FL_CONF['cache_expiration']);
                }
            }
        }
						
		switch( $autotag['tag'] ) {
            case 'flickr' :

				$id = $autotag['parm1'];
				$title = '';

				$photoalign = '';           // default alignment: none
				$photosize = 'thumbnail';   // default photo size
				if (!empty ($autotag['parm2'])) {
					$skip = 0;
					$px = explode (' ', trim ($autotag['parm2']));
					if (is_array ($px)) {
						foreach ($px as $part) {
							if (substr ($part, 0, 6) == 'align:') {
								$a = explode (':', $part);
								$a = strtolower ($a[1]);
								if (($a == 'left') || ($a == 'right')) {
									$photoalign='align="' . $a . '" ';
								}
								$skip++;
							} else if (substr ($part, 0, 5) == 'size:') {
								$s = explode (':', $part);
								$s = strtolower ($s[1]);
								if (($s == 'square') || ($s == 'thumbnail') ||
										($s == 'small') || ($s == 'medium')) {
									$photosize = ucwords ($s);
								}
								$skip++;
							} else {
								break;
							}
						}
						if ($skip == 0) {
							$title = trim ($autotag['parm2']);
						} else {
							if (count ($px) > $skip) {
								for ($i = 0; $i < $skip; $i++) {
									array_shift ($px);
								}
								$title = trim (implode (' ', $px));
							}
						}
					} else {
						$title = trim ($autotag['parm2']);
					}
				}

				$thumbnail = '';
				$photopage = '';

				$info = $flickr->photos_getInfo ($id);

				if (empty ($title) && !empty ($info['title'])) {
					$title = $info['title'];
				}

				$urls = $info['urls']['url'];
				if (is_array ($urls)) {
					foreach ($urls as $url) {
						if ($url['type'] == 'photopage') {
							$photopage = $url['_value'];
							break;
						}
					}
				}

				if (!empty ($photopage)) {
					$sizes = $flickr->photos_getSizes ($id);

					foreach ($sizes as $size) {
						if ($size['label'] == $photosize) {
							$thumbnail = '<img src="' . $size['source'] . '" width="'
								. $size['width'] . '" height="' . $size['height'] . '" '
								. 'border="0" ' . $photoalign . 'alt="' . $title
								. '" title="' . $title . '" class="flickr-photo">';
							break;
						}
					}
				}

				if (!empty ($thumbnail) && !empty ($photopage)) {
					$piclink = '<a href="' . $photopage . '">' . $thumbnail . '</a>';
					$content = str_replace ($autotag['tagstr'], $piclink, $content);
				} else if (!empty ($title) && !empty ($photopage)){
				    $piclink = '<a href="' . $photopage . '">' . $title . '</a>';
					$content = str_replace ($autotag['tagstr'], $piclink, $content);
				}
				return $content;
				break;
				
			case 'flickr_gallery' :
				$per_page = $autotag['parm1'];
				if ($per_page > 500) {
				    $per_page = 500;
				}
				
				$skip = 0;

				$px = explode (' ', trim ($autotag['parm2']));
				if (is_array ($px)) {
					foreach ($px as $part) {
						if (substr ($part, 0, 8) == 'user_id:') {
							$a = explode (':', $part);
							$user_id = $a[1];
							$skip++;
						} elseif (substr ($part, 0, 5) == 'tags:') {
							$a = explode (':', $part);
							$tags = $a[1];
							$skip++;
						} elseif (substr ($part, 0, 16) == 'min_upload_date:') {
							$a = explode (':', $part);
							$min_upload_date = $a[1];
							$skip++;
						} elseif (substr ($part, 0, 16) == 'max_upload_date:') {
							$a = explode(':', $part);
							$max_upload_date = $a[1];
							$skip++;
							$aSet = 1;
						} elseif (substr($part, 0, 5) == 'sort:') {
							$a = explode(':', $part);
							$sort = $a[1];
							$skip++;
						} elseif (substr($part, 0, 5) == 'bbox:') {
							$a = explode(':', $part);
							$bbox = $a[1];
							$skip++;
						} elseif (substr($part, 0, 12) == 'safe_search:') {
							$a = explode(':',$part);
							$safe_search = $a[1];
							$skip++;
						} elseif (substr ($part, 0, 13) == 'content_type:') {
							$a = explode (':', $part);
							$content_type = $a[1];
							$skip++;
						} elseif (substr ($part, 0, 9) == 'group_id:') {
							$a = explode (':', $part);
							$group_id = $a[1];
							$skip++;
						} elseif (substr ($part, 0, 4) == 'lat:' ) {
							$a = explode (':',$part);
							$slat = $a[1];
							$skip++;
						} elseif (substr ($part, 0, 4) == 'lon:') {
							$a = explode (':', $part);
							$lon = $a[1];
							$skip++;
						} elseif ( substr($part, 0, 13) == 'radius_units:' ) {
							$a = explode (':',$part);
							$radius_units = $a[1];
							$skip++;
						} elseif ( substr($part, 0, 7) == 'set_id:' ) {
							$a = explode (':',$part);
							$set_id = $a[1];
							$skip++;
						} elseif ( substr($part, 0, 5) == 'size:' ) {
							$a = explode (':',$part);
							$image_size = $a[1];
							$skip++;
						} else {
							break;
						}
					}
					if ($skip != 0) {
						if (count ($px) > $skip) {
							for ($i = 0; $i < $skip; $i++) {
								array_shift ($px);
							}
							$caption = trim (implode (' ', $px));
						} else {
							$caption = '';
						}
					} else {
						$caption = trim ($autotag['parm2']);
					}
				}
				
				//builds the gallery
				$search['user_id'] = $user_id;
				$search['tags'] = $tags;
				$search['min_upload_date'] = $min_upload_date;
				$search['max_upload_date'] = $max_upload_date;
				$search['min_taken_date'] = $min_taken_date;
				$search['max_taken_date'] = $max_taken_date;
				$search['sort'] = $sort;
				$search['bbox'] = $bbox;
				$search['safe_search'] = $safe_search;
				$search['content_type'] = $content_type;
				$search['group_id'] = $group_id;
				$search['lat'] = $lat;
				$search['lon'] = $lon;
				$search['radius_units'] = $radius_units;
				$search['per_page'] = $per_page;
				
				$sizes = array('s', 't', 'm', '-', 'z', 'b', 'o');
				if (!in_array($image_size, $sizes)) {
					$image_size = $_FL_CONF['mini_gallery_format'];
				} else {
					$image_size = '_' . $image_size;
				}
				
				$gallery = $flickr->getCached ("$user_id . $group_id . $tags");
				
				if ($gallery == false) {
					$gallery = $flickr->request ('flickr.photos.search', $search);
				    $flickr->cache ("$user_id . $group_id . $tags", $gallery);
				    $gallery = $flickr->parse_response ($gallery);
				} else {
					$gallery = $flickr->parse_response ($gallery);
				}

				$display = '<div class="flickr-mini-gallery">';
				if (is_array ($gallery['photos']['photo'])) {
					foreach ($gallery['photos']['photo'] as $item) {
						if (isset($item['server'])) $display .= '<a class="lightbox" href="http://static.flickr.com/'. $item['server'] . '/'. $item['id'] . '_'. $item['secret'] . '.jpg" alt="'. $item['id'] . '" title="'. $item['title'] . '"><img alt="'. $item['title'] . '" src="http://static.flickr.com/'. $item['server'] . '/'. $item['id'] . '_'. $item['secret'] . $image_size . '.jpg"></a>';
					}
				}
				$display .= '<p>' . $caption . '</p></div>';

				$content = str_replace ($autotag['tagstr'], $display, $content);
				return $content;
				break;
				
			
			case 'flickr_slideshow' :
			
			    $skip = 0;

				$px = explode (' ', trim ($autotag['parm2']));
				if (is_array ($px)) {
					foreach ($px as $part) {
						if ( substr($part, 0, 6) == 'width:' ) {
							$a = explode (':',$part);
							$width = $a[1];
							$skip++;
						} elseif ( substr($part, 0, 7) == 'height:' ) {
							$a = explode (':',$part);
							$height = $a[1];
							$skip++;
						} elseif ( substr($part, 0, 7) == 'set_id:' ) {
							$a = explode (':',$part);
							$set_id = $a[1];
							$skip++;
						} else {
							break;
						}
					}
					if ($skip != 0) {
						if (count ($px) > $skip) {
							for ($i = 0; $i < $skip; $i++) {
								array_shift ($px);
							}
							$caption = trim (implode (' ', $px));
						} else {
							$caption = '';
						}
					} else {
						$caption = trim ($autotag['parm2']);
					}
				}
			
			    //builds the slideshow
				$user_id = $autotag['parm1'];
				if ($width == '') {
				    $width = 400;
					$height = 300;
				} else if ($width != '100%') {
				    $height = $width * 0.75;
				}
				if ($set_id == ''){
					$slideshow = "<object width=\"$width\" height=\"$height\"> <param name=\"flashvars\" value=\"offsite=true&page_show_url=%2Fphotos%2F{$user_id}%2Fshow%2F&page_show_back_url=%2Fphotos%2{$user_id}%2F&user_id={$user_id}\"></param> <param name=\"movie\" value=\"http://www.flickr.com/apps/slideshow/show.swf?v=71649\"></param> <param name=\"allowFullScreen\" value=\"true\"></param><embed type=\"application/x-shockwave-flash\" src=\"http://www.flickr.com/apps/slideshow/show.swf?v=71649\" allowFullScreen=\"true\" flashvars=\"offsite=true&page_show_url=%2Fphotos%2F{$user_id}%2Fshow%2F&page_show_back_url=%2Fphotos%2F{$user_id}%2F&user_id={$user_id}\" width=\"$width\" height=\"$height\"></embed></object>";
				} else {
					$slideshow  = "<object width=\"$width\" height=\"$height\"> <param name=\"flashvars\" value=\"offsite=true&page_show_url=%2Fphotos%2F{$user_id}%2Fsets%2F{$set_id}%2Fshow%2F&page_show_back_url=%2Fphotos%2F{$user_id}%2Fsets%2F{$set_id}%2F&set_id={$set_id}\"></param> <param name=\"movie\" value=\"http://www.flickr.com/apps/slideshow/show.swf?v=71649\"></param> <param name=\"allowFullScreen\" value=\"true\"></param><embed type=\"application/x-shockwave-flash\" src=\"http://www.flickr.com/apps/slideshow/show.swf?v=71649\" allowFullScreen=\"true\" flashvars=\"offsite=true&page_show_url=%2Fphotos%2F{$user_id}%2Fsets%2F{$set_id}%2Fshow%2F&page_show_back_url=%2Fphotos%2F{$user_id}%2Fsets%2F{$set_id}%2F&set_id={$set_id}\" width=\"$width\" height=\"$height\"></embed></object>";
				}
				$content = str_replace ($autotag['tagstr'], $slideshow, $content);
				return $content;
				break;
		}
    }
}


/**
* Return plugin entry for "Command and Control" (moderation.php)
*
* @return  array  Array containing (plugin name, admin url, url of plugin icon)
*
*/
function plugin_cclabel_flickr()
{
    global $_CONF, $LANG_FLICKR_1;

    $retval = array();
    if (SEC_hasRights('flickr.admin')) {
        $retval = array($LANG_FLICKR_1['plugin_name'],
            $_CONF['site_admin_url'] . '/plugins/flickr/index.php',
            plugin_geticon_flickr());
    }

    return $retval;
}

/**
* Return plugin entry for the Admins Only block
*
* @return array Array containing (plugin name, plugin admin url, # of plugin items or '')
*/
function plugin_getadminoption_flickr()
{
    global $_CONF, $LANG_FLICKR_1;

    if (SEC_hasRights('flickr.admin')) {
        return array($LANG_FLICKR_1['plugin_name'],
            $_CONF['site_admin_url'] . '/plugins/flickr/index.php', 0);
    }
}

/**
* Returns the URL of the plugin's icon
*
* @return   string      URL of the icon
*
*/
function plugin_geticon_flickr()
{
    global $_CONF;

    // popular alternative location:
    // return $_CONF['site_url'] . '/flickr/images/flickr.png';

    return $_CONF['site_admin_url'] . '/plugins/flickr/images/flickr.png';
}

/**
 * Returns the current plugin code version
 *
 * @return string    plugin version
 */
function plugin_chkVersion_flickr()
{
    global $_CONF;

    require_once $_CONF['path'] . 'plugins/flickr/autoinstall.php';

    $inst_parms = plugin_autoinstall_flickr('flickr');

    return $inst_parms['info']['pi_version'];
}

/**
* Update the plugin
*
* @return   int     Number of message to display or true for success
*
*/
function plugin_upgrade_flickr()
{
    global $_CONF, $_TABLES;

    $currentVersion = DB_getItem($_TABLES['plugins'], 'pi_version',
                                    "pi_name = 'flickr'");
    $code_version = plugin_chkVersion_flickr();
    if ($currentVersion == $code_version) {
        // nothing to do
        return true;
    }

    require_once $_CONF['path'] . 'plugins/flickr/autoinstall.php';
	require_once $_CONF['path'] . 'plugins/flickr/install_defaults.php';
	require_once $_CONF['path_system'] . 'classes/config.class.php';

    if (! plugin_compatible_with_this_version_flickr('flickr')) {
        return 3002;
    }

    // other update code goes here
	
	switch( $currentVersion ) {
        
		case '0.1.0' :
		case '0.1.1' :
		case '0.1.2' :
		case '0.2.0' :
		case '1.0.0' :
			DB_query("UPDATE {$_TABLES['plugins']} SET pi_homepage='http://geeklog.fr' WHERE pi_name='flickr",1);

			$c = config::get_instance();						
			
			//This is main subgroup #0
		    $c->add('sg_0', NULL, 'subgroup', 0, 0, NULL, 0, true, 'flickr');
		
		    //This is fieldset #1  in subgroup #0   
		    $c->add('fs_01', NULL, 'fieldset', 0, 0, NULL, 0, true, 'flickr');
            $c->add('use_cache', $_FL_DEFAULT['use_cache'],
                    'select', 0, 0, 0, 10, true, 'flickr');
            $c->add('cache_type', $_FL_DEFAULT['cache_type'], 'select',
		            0, 0, 0, 20, true, 'flickr');
		    $c->add('cache_expiration', $_FL_DEFAULT['cache_expiration'], 'text',
                    0, 0, 0, 30, true, 'flickr');	
		    $c->add('cache_table_name', $_FL_DEFAULT['cache_table_name'], 'text',
                    0, 0, 0, 40, true, 'flickr');
		    $c->add('cache_fs_name', $_FL_DEFAULT['cache_fs_name'], 'text',
                    0, 0, 0, 50, true, 'flickr');
				
		    //This is fieldset #2  in subgroup #0   
		    $c->add('fs_02', NULL, 'fieldset', 0, 1, NULL, 0, true, 'flickr');
            $c->add('use_CND', $_FL_DEFAULT['use_CND'],
                    'select', 0, 1, 0, 60, true, 'flickr');
		
		    //This is fieldset #3  in subgroup #0   
		    $c->add('fs_03', NULL, 'fieldset', 0, 2, NULL, 0, true, 'flickr');
            $c->add('mini_gallery_format', $_FL_DEFAULT['mini_gallery_format'],
                    'select', 0, 2, 0, 70, true, 'flickr');
		case '1.1.0' :
        case '1.1.1' :
			$c = config::get_instance();	
			//This is fieldset #4  in subgroup #0   
		    $c->add('fs_04', NULL, 'fieldset', 0, 3, NULL, 0, true, 'flickr');
            $c->add('flickr_api_key', '', 'text', 0, 3, 0, 80, true, 'flickr');
		case '1.1.2' :
		    $c = config::get_instance();
			$c->del('cache_table_name', 'flickr');
			$c->del('fs_02', 'flickr');
			$c->del('use_CND', 'flickr');
		case '1.2' :
		case '1.2.1':
		case '1.2.2':
        case '1.2.3':

        default :		
			// update plugin version number
            $inst_parms = plugin_autoinstall_flickr('flickr');
            $pi_gl_version = $inst_parms['info']['pi_gl_version'];
            DB_query("UPDATE {$_TABLES['plugins']} SET pi_version = '$code_version', pi_gl_version = '$pi_gl_version' WHERE pi_name = 'flickr'");
	        COM_errorLog( "Updated Flickr plugin from v$currentVersion to v$code_version", 1 );
			/* This code is for statistics ONLY */
            $message =  'Completed flickr plugin upgrade: ' . date('m d Y',time()) . "   AT " . date('H:i', time()) . "\n";
            $message .= 'Site: ' . $_CONF['site_url'] . ' and Sitename: ' . $_CONF['site_name'] . "\n";
            COM_mail("ben@geeklog.fr","Updated flickr plugin from v$currentVersion to v$code_version",$message);
    }
	
    return true;
}

/**
* Automatic uninstall function for plugins
*
* This code is automatically uninstalling the plugin.
* It passes an array to the core code function that removes
* tables, groups, features and php blocks from the tables.
* Additionally, this code can perform special actions that cannot be
* foreseen by the core code (interactions with other plugins for example)
*
* @return   array   Plugin information
*
*/
function plugin_autouninstall_flickr()
{
    $out = array (
        /* give the name of the tables, without $_TABLES[] */
        'tables' => array('flickr_cache'),
        /* give the full name of the group, as in the db */
        'groups' => array('Flickr Admin'),
        /* give the full name of the feature, as in the db */
        'features' => array('flickr.admin'),
        /* give the full name of the block, including 'phpblock_', etc */
        'php_blocks' => array(),
        /* give all vars with their name */
        'vars' => array()
    );

    return $out;
}

function plugin_getheadercode_flickr()
{
    global $_CONF, $_FL_CONF, $_SCRIPTS;
	
    return;
}

?>
