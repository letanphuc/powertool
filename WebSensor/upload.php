<?php
// NOte: You need to change user and/or group ownership/
// chmod 777 -R /var/www/powertool/
// chown www-data:www-data -R /var/www/powertool/

$target_dir = "/var/www/powertool/WebSensor/Upload/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
    $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
}
// Allow certain file formats
if($imageFileType != "tar" ) {
    echo "Sorry, only tar file are allowed.\n";
    $uploadOk = 0;
}
// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        echo "Update new version for web, please go back to main page";
        try {
            $phar = new PharData( $target_file);
            $phar->extractTo('../',null, true);
        }
        catch (Exception $e) {
    // handle errors
            echo $e;
        }
    } else {
        echo "Sorry, there was an error uploading your file, please go back to main page";
    }
}
?>