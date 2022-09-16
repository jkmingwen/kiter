library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- top-level entity declaration
entity axi_select2 is
  port (
    clk : in std_logic;
    rst : in std_logic;

    op_in_ready_0 : out std_logic;
    op_in_valid_0 : in std_logic;
    op_in_data_0 : in std_logic_vector(33 downto 0) := (others => '0');

    op_in_ready_1 : out std_logic;
    op_in_valid_1 : in std_logic;
    op_in_data_1 : in std_logic_vector(33 downto 0) := (others => '0');

    op_in_ready_2 : out std_logic;
    op_in_valid_2 : in std_logic;
    op_in_data_2 : in std_logic_vector(33 downto 0) := (others => '0');

    op_out_ready_0 : in std_logic;
    op_out_valid_0 : out std_logic;
    op_out_data_0 : out std_logic_vector(33 downto 0) := (others => '0')
    );
end axi_select2;

architecture connections of axi_select2 is

  component select2 is
    port( s  : in std_logic_vector(33 downto 0);
          x1 : in std_logic_vector(33 downto 0);
          x2 : in std_logic_vector(33 downto 0);
          y  : out std_logic_vector(33 downto 0) );
  end component;

begin
  op_in_ready_0 <= op_out_ready_0;
  op_in_ready_1 <= op_out_ready_0;
  op_in_ready_2 <= op_out_ready_0;
  op_out_valid_0 <= op_in_valid_0 and op_in_valid_1 and op_in_valid_2;

  selecting : select2 port map ( s  => op_in_data_0,
                                 x1 => op_in_data_1,
                                 x2 => op_in_data_2,
                                 y  => op_out_data_0 );


end connections;
