<?php
require('assets/Control.php');
if(isset($_POST['submit'])){
    $file = $_FILES['file'];



    // Count total files
    $countfiles = count($_FILES['file']['name']);

    // Looping all files
    for($i=0;$i<$countfiles;$i++){
      $fileName = $_FILES['file']['name'][$i];
      $fileTmpName = $_FILES['file']['tmp_name'][$i];
      $fileSize = $_FILES['file']['size'][$i];
      $fileError= $_FILES['file']['error'][$i];
      $fileType = $_FILES['file']['type'][$i];





      $fileExt = explode('.', $fileName);
      $fileActualExt = strtolower(end($fileExt));


      $allowed = array('sh');

      if(in_array($fileActualExt, $allowed)){
          if ($fileError === 0){
       if($fileSize < 1000 ){ // this is in bits
         // upload_max_filesize = 1000M;    post_max_size = 1000M; add this to the php.ini as that you are able to upload larger files greater then 8MB

             
                          $filepath = '../HID/'.$fileName;
                          move_uploaded_file($fileTmpName, $filepath);      // if you get to here the upload worked
                          $upload = true;
                          echo shell_exec("sudo chmod +x ../HID/* 2>&1");
                          echo shell_exec("sudo chmod 777 ../HID/* 2>&1");
                                  }else{echo"Your File is to big !";$upload = false;}
                                }else{echo"there was a error uploading";$upload = false;}
                              }else{echo"This file type is not allowed";$upload = false;}


                            }

}
if( $upload == true){
    echo'
    <script>
    alert("Upload successful please wait ");
    </script>
    ';

}else{
    echo'
    <script>
    alert("Upload encounted a Error, please wait and try again ");
    </script>
    ';   
}
home();
?>