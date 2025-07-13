<?php
    $file = dirname(dirname(__FILE__)) . '/d_struct/data.json'; // dirname(dirname(__FILE__)) goes one directory up from there
    // Locating the JSON File; __FILE__ is a magic constant in PHP that gives the full path to the current script.
    
    $dataJson = file_get_contents($file); //  Reading the JSON File
    
    $data = json_decode($dataJson, true); // Decoding the JSON Data; $data is now a PHP array representing the JSON structure.

    $entries = $data['entries']; // Extracting the Entries; $entries is now an array of all the entries from the JSON

    
    usort($entries, function($a, $b) {  // usort sorts the $entries array in place, using the provided comparison function
        return $a['id'] - $b['id'];
    });
   // Outputting Each Entry as a Table Row
    foreach ($entries as $entry) {    // For each entry, a row is added to the HTML table with the entry’s details and an associated image.
        echo "<tr>";
        echo "<td>" . $entry['id'] . "</td>";
        echo "<td>" . $entry['description'] . "</td>";
        echo "<td>" . $entry['price'] . "</td>";
        echo "<td>" . $entry['phone'] . "</td>";
        echo "<td> <img src='images/{$entry['id']}.jpeg' width='115' height='115' alt='car image'/>  </td>";
        echo "</tr>";
    }
?>