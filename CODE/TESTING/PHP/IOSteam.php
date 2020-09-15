<?PHP
// from https://stackoverflow.com/questions/8370628/php-shell-exec-with-realtime-updating

// for testing non block buffer
if( ($fp = popen("cat IOSteam.php ", "r")) ) {
    while( !feof($fp) ){
        echo fread($fp, 1024);
        flush(); // you have to flush buffer
    }
    fclose($fp);
}


?>
