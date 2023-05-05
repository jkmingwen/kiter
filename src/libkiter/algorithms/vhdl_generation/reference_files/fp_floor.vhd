library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- top-level entity declaration
entity fp_floor is
  port ( clk : in std_logic;
         rst : in std_logic;

         op_in_ready_0 : out std_logic;
         op_in_valid_0 : in std_logic;
         op_in_data_0  : in std_logic_vector(33 downto 0) := (others => '0');

         op_out_ready_0  : in std_logic;
         op_out_valid_0  : out std_logic;
         op_out_data_0   : out std_logic_vector(33 downto 0) := (others => '0') );
end fp_floor;

architecture connections of fp_floor is
  component fp_floor_f$FREQ is
    port (clk : in std_logic;
          rst : in std_logic;

          op_in_ready_0 : out std_logic;
          op_in_valid_0 : in std_logic;
          op_in_data_0  : in std_logic_vector(33 downto 0) := (others => '0');

          op_out_ready_0  : in std_logic;
          op_out_valid_0  : out std_logic;
          op_out_data_0   : out std_logic_vector(33 downto 0) := (others => '0') );
  end component;

begin

  fp_floor_implementation : fp_floor_f$FREQ port map ( clk => clk,
                                                     rst => rst,
                                                     op_in_ready_0 => op_in_ready_0,
                                                     op_in_valid_0 => op_in_valid_0,
                                                     op_in_data_0 => op_in_data_0,
                                                     op_out_ready_0 => op_out_ready_0,
                                                     op_out_valid_0 => op_out_valid_0,
                                                     op_out_data_0 => op_out_data_0 );

end connections;
