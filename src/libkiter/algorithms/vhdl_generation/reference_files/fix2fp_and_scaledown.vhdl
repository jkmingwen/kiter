library ieee;
use ieee.std_logic_1164.all;

entity fix2fp_and_scaledown is
    generic (
        i2s_bit_width : natural := 24;
        fp_bit_width : natural := 34;
        bit_depth : natural := 2;
        scale_factor : std_logic_vector(33 downto 0) := "0100110100000000000000000000000001"); -- 1 / (2^23 - 1)
    port (
        clk : in std_logic;
        rst : in std_logic;
        i2s_in : in std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0');
        fp_out : out std_logic_vector(fp_bit_width - 1 downto 0) := (others => '0'));
end fix2fp_and_scaledown;

architecture Behavioral of fix2fp_and_scaledown is

component fix2fp_flopoco_f$OP_FREQ is
    port  (clk, rst : in std_logic;
           I : in  std_logic_vector(23 downto 0);
           O : out  std_logic_vector(8+23+2 downto 0)   );
end component;

component fp_prod_flopoco_f$OP_FREQ is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          Y : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end component;

signal i2s_to_fp : std_logic_vector(fp_bit_width - 1 downto 0);

begin

i2s_to_fp_convertor : fix2fp_flopoco_f$OP_FREQ
    port map (
        clk => clk,
        rst => rst,
        I => i2s_in,
        O => i2s_to_fp
    );

fp_prod : fp_prod_flopoco_f$OP_FREQ
    port map (
        clk => clk,
        X => i2s_to_fp,
        Y => scale_factor,
        R => fp_out
    );

end Behavioral;
