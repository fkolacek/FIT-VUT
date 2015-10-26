#!/usr/bin/php

<?php

require "db.php";

mysql_query("DELETE FROM hits") or die(mysql_error());
mysql_query("DELETE FROM tests") or die(mysql_error());
