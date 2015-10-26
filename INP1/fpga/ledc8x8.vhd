--- Projekt: INP 1 - Maticovy displej
--- Autor:   Frantisek Kolacek (xkolac12@stud.fit.vutbr.cz)
--- Datum:   28. 10. 2012

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

-- Definice rozhrani pro praci s maticovym displejem
entity ledc8x8 is
port (
    SMCLK: in std_logic;
    RESET: in std_logic;
    ROW: out std_logic_vector(0 to 7);
    LED: out std_logic_vector(0 to 7)
);
end ledc8x8;

--- Definice architektury
architecture behav of ledc8x8 is

--- Signal pro novy clock
signal customCLK: std_logic;

--- Registr pro uchovani hodnoty citace
signal customReg: std_logic_vector(7 downto 0);

--- Registr pro adresaci radku
signal customRow: std_logic_vector(7 downto 0);

begin
    --- Proces pro vytvoreni vlastniho clock
    process(SMCLK, RESET, customCLK, customReg)
    begin
        if RESET = '1' then
            customReg <= (others => '0');
        elsif SMCLK'event and SMCLK = '1' then
            customReg <= customReg + 1;
        end if;
        
        customCLK <= customReg(7);
    end process;

    --- Proces pro obsluhu kruhoveho registru pracujiciho s nabeznou hranou
    process(customCLK, RESET)
    begin
        if RESET = '1' then
            customRow <= "01111111";
        elsif customCLK'event AND customCLK = '1' then
            customRow <= customRow(0) & customRow(7 downto 1);
        end if;
    end process;
    
    --- Switch (dekoder) signalu pro rozsveceni diod
    process(customRow)
    begin
        case customRow is
            when "01111111" =>  LED <= "11110000";
            when "10111111" =>  LED <= "10000000";
            when "11011111" =>  LED <= "11100000";
            when "11101111" =>  LED <= "10001001";
            when "11110111" =>  LED <= "10001010";
            when "11111011" =>  LED <= "10001100";
            when "11111101" =>  LED <= "00001010"; 
            when "11111110" =>  LED <= "00001001";
            when others    =>  LED <= "00000000"; 
        end case;
    end process;
    
    ROW <= customRow;     
end behav;
