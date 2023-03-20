library ieee;
use ieee.std_logic_1164.all;

entity fp2fix_and_scaleup is
    generic (
        i2s_bit_width : natural := 24;
        fp_bit_width : natural := 34;
        bit_depth : natural := 2;
        scale_factor : std_logic_vector(33 downto 0) := "0101001010111111111111111111111110"); -- 2^23 - 1
    port (
        clk : in std_logic;
        rst : in std_logic;
        fp_in : in std_logic_vector(fp_bit_width - 1 downto 0) := (others => '0');
        i2s_out : out std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0'));
end fp2fix_and_scaleup;

architecture Behavioral of fp2fix_and_scaleup is

component FP2Fix_8_23_0_23_S_T_F125_uid2 is
    port  (clk : in std_logic;
           I : in  std_logic_vector(8+23+2 downto 0);
           O : out  std_logic_vector(23 downto 0)   );
end component;

component FPMult_8_23_8_23_8_23_uid2_F125_uid3 is
    port (clk : in std_logic;
          X : in  std_logic_vector(8+23+2 downto 0);
          Y : in  std_logic_vector(8+23+2 downto 0);
          R : out  std_logic_vector(8+23+2 downto 0)   );
end component;

signal fp_to_i2s : std_logic_vector(fp_bit_width - 1 downto 0);

begin

fp_to_i2s_convertor : FP2Fix_8_23_0_23_S_T_F125_uid2
    port map (
        clk => clk,
        I => fp_to_i2s,
        O => i2s_out
    );

fp_prod : FPMult_8_23_8_23_8_23_uid2_F125_uid3
    port map (
        clk => clk,
        X => fp_in,
        Y => scale_factor,
        R => fp_to_i2s
    );


end Behavioral;
