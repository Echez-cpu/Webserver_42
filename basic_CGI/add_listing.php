<?php
$file = dirname(dirname(__FILE__)) . '/d_struct/data.json';
$dataJson = file_get_contents($file); // Reads the entire contents of the JSON file into a string variable $dataJson

$data = json_decode($dataJson, true);


$post_data_file = $argv[1]; // Grabs the first command-line argument set in the code (should be a filename containing POST data, e.g., "description=abc&price=10&phone=123")
$post_data = file_get_contents($post_data_file);
parse_str($post_data, $_POST); // Parses the URL-encoded string into the $_POST array, so you can use $_POST['description'].

$entries = $data['entries'];

$description = $_POST['description'];
$price = $_POST['price'];
$phone = $_POST['phone'];

$usedIds = array_column($entries, 'id');
sort($usedIds);
for ($i = 1; $i <= count($entries) + 1; $i++) { // Sorts the IDs in ascending order
    if (!in_array($i, $usedIds)) {
        $id = $i;
        break;
    }
}

echo $id;

// Add the New Entry; Updates the main data array with the new list of entries...
// Adds a new entry (as an associative array) to the $entries array...

$entries[] = array(
  'id' => (string)$id,
  'description' => $description,
  'price' => $price,
  'phone' => $phone,
);

$data['entries'] = $entries;
// Save the Data Back to the File; Converts the updated data array back into a JSON string.
$dataJson = json_encode($data);
// Writes the new JSON string back to the original data file, saving the change..
file_put_contents($file , $dataJson);

?>