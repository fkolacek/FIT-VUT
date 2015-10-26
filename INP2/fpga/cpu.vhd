-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2011 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Zdenek Vasicek <vasicek AT fit.vutbr.cz>
--            Frantisek Kolacek <xkolac12 AT stud.fit.vutbr.cz>
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity S_DEClaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru

   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti

   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti

   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data

   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture S_DEClaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

--
  -- Signaly pro praci s pocitadlem
  signal counterRegister : std_logic_vector (7 downto 0);
  signal counterSet : std_logic;
  signal counterInc : std_logic;
  signal counterDec : std_logic;
  
  -- Signaly pro praci s ukazatelem
  signal pointerRegister : std_logic_vector (9 downto 0);
  signal pointerSet : std_logic;
  signal pointerInc : std_logic;
  signal pointerDec : std_logic;
  
  -- Signaly pro praci s pcRegister
  signal pcRegister : std_logic_vector (11 downto 0);
  signal pcSet : std_logic;
  signal pcInc : std_logic;
  signal pcDec : std_logic;
  
  -- Instrukce v jazyce brainfuck
  type instructions is (I_IDLE, I_INC, I_DEC, I_MOVE_LEFT, I_MOVE_RIGHT, I_OUTPUT, I_INPUT, I_LOOP_START, I_LOOP_END, I_NOOP);
  signal instructionsPointer : instructions;
  
  -- Stavy procesoru
  type cpuStates is (S_IDLE, S_LOAD, S_STOP, S_INC, S_DEC, S_IN, S_OUT, S_LOOP_START, S_LOOP_END, S_LOOP_CONT, S_LOOP_CONT_END, S_LOOP_REV, S_LOOP_REV_END);
  signal cpuPresentState : cpuStates;
  signal cpuNextState : cpuStates; 
--
begin
  CODE_ADDR <= pcRegister;
  DATA_ADDR <= pointerRegister;

  -- Proces pro nacteni a dekodovani instrukce
  cpuLoadInstruction: process (CODE_DATA)
  begin
    case CODE_DATA is
      -- +
      when X"2B" =>
        instructionsPointer <= I_INC;
      -- ,
      when X"2C" =>
        instructionsPointer <= I_INPUT;
      -- -
      when X"2D" =>
        instructionsPointer <= I_DEC;
      -- .
      when X"2E" =>
        instructionsPointer <= I_OUTPUT;
      -- <
      when X"3C" =>
        instructionsPointer <= I_MOVE_LEFT;
      -- >
      when X"3E" =>
        instructionsPointer <= I_MOVE_RIGHT;
      -- [
      when X"5B" =>
        instructionsPointer <= I_LOOP_START;
      -- ]
      when X"5D" =>
        instructionsPointer <= I_LOOP_END;
      -- EOF
      when X"00" =>
        instructionsPointer <= I_NOOP;
      when others =>
        instructionsPointer <= I_IDLE;
    end case;
  end process;

  -- Proces pro pocitadlo counter
  counter: process (RESET, CLK)
  begin
    if (RESET = '1') then
      counterRegister <= "00000000";
    elsif (CLK'event and CLK = '1') then
      if (counterInc = '1') then
        counterRegister <= counterRegister + 1;
      elsif (counterDec = '1') then
        counterRegister <= counterRegister - 1;
      elsif (counterSet = '1') then
        counterRegister <= "00000001";
      end if;
    end if;
  end process;

  -- Proces pro pocitadlo pointer
  pointer: process (RESET, CLK)
  begin
    if (RESET = '1') then
      pointerRegister <= "0000000000";
    elsif (CLK'event and CLK = '1') then
      if (pointerInc = '1') then
        pointerRegister <= pointerRegister + 1;
      elsif (pointerDec = '1') then
        pointerRegister <= pointerRegister - 1;
      end if;
    end if;
  end process;
  
  -- Proces pro pocitadlo pc
  pc: process (RESET, CLK)
  begin
    if (RESET = '1') then
      pcRegister <= "000000000000";
    elsif (CLK'event and CLK = '1') then
      if (pcInc = '1') then
        pcRegister <= pcRegister + 1;
      elsif (pcDec = '1') then
        pcRegister <= pcRegister - 1;
      end if;
    end if;
  end process;

  -- Present state
  cpuPresentStateProcess: process (RESET, CLK, EN)
  begin
    if (RESET = '1') then
      cpuPresentState <= S_IDLE;
    elsif (CLK'event and CLK = '1') then
      if (EN = '1') then
        cpuPresentState <= cpuNextState;
      end if;
    end if;
  end process;

  -- Next state
  cpuNextStateProcess: process (CODE_DATA, DATA_RDATA, EN, IN_DATA, IN_VLD, OUT_BUSY, cpuPresentState, instructionsPointer, counterRegister)
  begin

    CODE_EN <= '0';
    DATA_EN <= '0';

    OUT_WE <= '0';
    
    IN_REQ <= '0';

    counterInc <= '0';
    counterDec <= '0';
    counterSet <= '0';

    pointerSet <= '0';
    pointerInc <= '0';
    pointerDec <= '0';
    
    pcSet <= '0';
    pcInc <= '0';
    pcDec <= '0';
    
    cpuNextState <= S_IDLE;

    case cpuPresentState is
      -- Pocatecni stav
      when S_IDLE =>
        CODE_EN <= '1';
        cpuNextState <= S_LOAD;
      -- Konecny stav
      when S_STOP =>
        cpuNextState <= S_STOP;
      -- Inc
      when S_INC => 
        DATA_WDATA <= DATA_RDATA + 1;
        DATA_RDWR <= '1';
        DATA_EN <= '1';
        pcInc <= '1';
        cpuNextState <= S_IDLE;
      -- Dec
      when S_DEC =>
        DATA_WDATA <= DATA_RDATA - 1;
        DATA_RDWR <= '1';
        DATA_EN <= '1';
        pcInc <= '1';
        cpuNextState <= S_IDLE;
      -- Out
      when S_OUT =>
        if (OUT_BUSY = '0') then
          OUT_DATA <= DATA_RDATA;
          OUT_WE <= '1';
          pcInc <= '1';
          cpuNextState <= S_IDLE;
        else
          cpuNextState <= S_OUT;
        end if;
      -- In
      when S_IN =>
        if (IN_VLD = '1') then
          DATA_RDWR <= '1';
          DATA_WDATA <= IN_DATA;
          DATA_EN <= '1';
          pcInc <= '1';
          cpuNextState <= S_IDLE;
        else
          IN_REQ <= '1';
          cpuNextState <= S_IN;
        end if;
      -- Nacani a dekodovani instrukci
      when S_LOAD => 
        case instructionsPointer is
          -- +
          when I_INC =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            cpuNextState <= S_INC;
          -- -
          when I_DEC =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            cpuNextState <= S_DEC;
          -- <
          when I_MOVE_LEFT =>
            pointerDec <= '1';
            pcInc <= '1';
            cpuNextState <= S_IDLE;
          -- >
          when I_MOVE_RIGHT =>
            pointerInc <= '1';
            pcInc <= '1';
            cpuNextState <= S_IDLE;
          -- [
          when I_LOOP_START =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            cpuNextState <= S_LOOP_START;
          -- ]
          when I_LOOP_END =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            cpuNextState <= S_LOOP_END;
          -- .
          when I_OUTPUT =>
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            cpuNextState <= S_OUT;
          -- ,
          when I_INPUT =>
            IN_REQ <= '1';
            cpuNextState <= S_IN;
          -- EOF
          when I_NOOP =>
            cpuNextState <= S_STOP;
          --
          when others =>
            pcInc <= '1';
            cpuNextState <= S_IDLE;            
        end case;
      -- Loop start 
      when S_LOOP_START =>
        if (DATA_RDATA = 0) then
          counterInc <= '1';
          pcInc <= '1';
          cpuNextState <= S_LOOP_CONT_END;
        else
          pcInc <= '1';
          cpuNextState <= S_IDLE;
        end if;
      -- Loop end
      when S_LOOP_END =>
        if (DATA_RDATA = 0) then
          pcInc <= '1';
          cpuNextState <= S_IDLE;
        else
          pcDec <= '1';
          counterSet <= '1';
          cpuNextState <= S_LOOP_REV_END;
        end if;
      -- Loop forward
      when S_LOOP_CONT =>
        if (counterRegister = 0) then
          cpuNextState <= S_IDLE;
        elsif (instructionsPointer = I_LOOP_END) then
          pcInc <= '1';
          counterDec <= '1';
          cpuNextState <= S_LOOP_CONT_END;
        elsif (instructionsPointer = I_LOOP_START) then
          pcInc <= '1';
          counterInc <= '1';
          cpuNextState <= S_LOOP_CONT_END;
        else
          pcInc <= '1';
          cpuNextState <= S_LOOP_CONT_END;
        end if;
      -- Loop rev
      when S_LOOP_REV =>
        if (instructionsPointer = I_LOOP_END) then
          counterInc <= '1';
          pcDec <= '1';
          cpuNextState <= S_LOOP_REV_END;
        elsif (instructionsPointer = I_LOOP_START and counterRegister = 1) then
          counterDec <= '1';
          cpuNextState <= S_IDLE;
        elsif (instructionsPointer = I_LOOP_START) then
          pcDec <= '1';
          counterDec <= '1';
          cpuNextState <= S_LOOP_REV_END;
        else
          pcDec <= '1';
          CODE_EN <= '1';
          cpuNextState <= S_LOOP_REV_END;
        end if;
      -- Loop end forward
      when S_LOOP_CONT_END =>
        CODE_EN <= '1';
        cpuNextState <= S_LOOP_CONT;
      -- Loop end rev
      when S_LOOP_REV_END =>
        CODE_EN <= '1';
        cpuNextState <= S_LOOP_REV;
      when others => 
        cpuNextState <= S_IDLE;
    end case; -- case present state
  end process;
end behavioral;
