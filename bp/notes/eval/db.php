<?php

  DEFINE("DBHOST", "localhost");
  DEFINE("DBUSER", "root");
  DEFINE("DBPASS", "");
  DEFINE("DBNAME", "wavsep");

  date_default_timezone_set('Europe/Prague');

  $dbHandler = mysql_connect(DBHOST, DBUSER, DBPASS) or die(mysql_error());
  mysql_select_db(DBNAME, $dbHandler) or die(mysql_error());
  mysql_query("SET NAMES utf8", $dbHandler) or die(mysql_error());
