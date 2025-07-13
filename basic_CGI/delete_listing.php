<?php
$file = dirname(dirname(__FILE__)) . '/d_struct/data.json';
$dataJson = file_get_contents($file);

$data = json_decode($dataJson, true);


$entries = $data['entries'];

$post_data_file = $argv[1];
$post_data = file_get_contents($post_data_file);
parse_str($post_data, $_POST);

$id = $_POST['id'];

$index = -1;
for ($i = 0; $i < count($entries); $i++) {
    if ($entries[$i]['id'] == $id) {
        $index = $i;
        break;
    }
}

if ($index != -1) {
    array_splice($entries, $index, 1);
}

$data['entries'] = $entries;

$dataJson = json_encode($data);

file_put_contents($file , $dataJson);

?>