<?php
  // testovaci program
  $ahoj = put_string("hello! insert number 10 for exp and string test, any other number for functions : ");
  $a = get_string();
  $a = intval($a);
  if($a === 10  )
  {
    $a = $a+10*15*$a/25-36*25/15*6+8;
    $vysledek = put_string("Value should be : -282 : ",$a);
    $string = put_string("volvo give diretide!");
    $temp = put_string("automobile facturer involved in recent DOTA event (lowercase): ");
    $search = get_string();
    $temp_ret = find_string($string,$search);
    $foo = put_string("String should start at 0: ", $temp_ret );
    $smth = put_string("insert five different letters: ");
    $sort = get_string();
    $sort = sort_string();
    $smth = put_string("Sorted letters: ", $sort);
  }
  else
  {
    $f1 = put_string("how you doin'? : ");
    $concatenate = get_string();
    function test_function($z)
    {
      
      
      $foo = 10;
      while ($foo > 0)
      {
        $f1 = put_string("final countdown: ", $foo);
        
        $concatenate = $concatenate.$concatenate;
        $foo = $foo - 1;
      }
    }
    test_function($concatenate);
    $f1 = put_string("really long (should be 10 times concatenated) description of how you doin': ",$concatenate);
  }
  
  $ahoj = put_string("test complete! should not crash nor return error code other than 0");

