-- Pipeline depth: 5 cycles

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
library std;
use std.textio.all;
library work;

entity fp_floor_flopoco_f250 is
   port ( clk, rst : in std_logic;
          X : in  std_logic_vector(33 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end entity;

architecture Behavioral of fp_floor_flopoco_f250 is
    component float2int_flopoco_f250 is
        port ( clk : in std_logic;
               X : in std_logic_vector(33 downto 0);
               R : out std_logic_vector(8+23+2 downto 0));
    end component;

    component int2float_flopoco_f250 is
        port ( clk, rst : in std_logic;
               X : in std_logic_vector(33 downto 0);
               R : out std_logic_vector(8+23+2 downto 0));
    end component;

signal intermediateR : std_logic_vector(33 downto 0);

begin

fp2fix : float2int_flopoco_f250
    port map (clk => clk,
              X => X,
              R => intermediateR);

fix2fp : int2float_flopoco_f250
    port map (clk => clk,
              rst => rst,
              X => intermediateR,
              R => R);

end Behavioral;
