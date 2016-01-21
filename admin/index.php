<?php

/* Reminder: always indent with 4 spaces (no tabs). */
// +---------------------------------------------------------------------------+
// | Flickr Plugin 1.0                                                         |
// +---------------------------------------------------------------------------+
// | index.php                                                                 |
// |                                                                           |
// | Plugin administration page                                                |
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

require_once '../../../lib-common.php';
require_once '../../auth.inc.php';

$display = '';

// Ensure user even has the rights to access this page
if (! SEC_hasRights('flickr.admin')) {
    $display .= COM_siteHeader('menu', $MESSAGE[30])
             . COM_showMessageText($MESSAGE[29], $MESSAGE[30])
             . COM_siteFooter();

    // Log attempt to access.log
    COM_accessLog("User {$_USER['username']} tried to illegally access the Flickr plugin administration screen.");

    echo $display;
    exit;
}


// MAIN
$display .= COM_siteHeader('menu', $LANG_FLICKR_1['plugin_name']);
$display .= COM_startBlock($LANG_FLICKR_1['plugin_name']);
$display .= '<p><img src="' . $_CONF['site_admin_url'] . '/plugins/flickr/images/flickr.png" style="vertical-align:middle;border:1px solid #EEEEEE; margin:5px;" alt=""> ' 
		 . $LANG_FLICKR_1['plugin_conf'] . ' <a href="#" onclick="document.flickr_conf_link.submit()">'. $LANG_FLICKR_1['online']
         . '</a>. ' . $LANG_FLICKR_1['plugin_doc'] . ' <a href="http://geeklog.fr/downloads/index.php/flickr" target="_blank">'. $LANG_FLICKR_1['online']
		 . '</a>. ' . '</p>';
$display .="<form name='flickr_conf_link' action='" . $_CONF['site_admin_url'] . "/configuration.php' method='POST'>
    <input type='hidden' name='conf_group' value='flickr'></form>";
 
$display .= COM_endBlock();
$display .= COM_siteFooter();

echo $display;

?>
