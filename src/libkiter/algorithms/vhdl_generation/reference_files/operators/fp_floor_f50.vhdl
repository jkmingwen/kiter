-- note that this requires the implementations of both float2int and int2float
-- within their respective handshake protocols
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity fp_floor_f50 is
    port (clk : in std_logic;
          rst : in std_logic;

          op_in_ready_0 : out std_logic;
          op_in_valid_0 : in std_logic;
          op_in_data_0  : in std_logic_vector(33 downto 0) := (others => '0');

          op_out_ready_0  : in std_logic;
          op_out_valid_0  : out std_logic;
          op_out_data_0   : out std_logic_vector(33 downto 0) := (others => '0')   );
end entity;

architecture arch of fp_floor_f50 is
    component float2int is
        port ( clk : in std_logic;
               rst : in std_logic;

               op_in_ready_0 : out std_logic;
               op_in_valid_0 : in std_logic;
               op_in_data_0  : in std_logic_vector(33 downto 0) := (others => '0');

               op_out_ready_0  : in std_logic;
               op_out_valid_0  : out std_logic;
               op_out_data_0   : out std_logic_vector(33 downto 0) := (others => '0') );
    end component;

    component int2float is
        port ( clk : in std_logic;
               rst : in std_logic;

               op_in_ready_0 : out std_logic;
               op_in_valid_0 : in std_logic;
               op_in_data_0  : in std_logic_vector(33 downto 0) := (others => '0');

               op_out_ready_0  : in std_logic;
               op_out_valid_0  : out std_logic;
               op_out_data_0   : out std_logic_vector(33 downto 0) := (others => '0') );
    end component;

    signal f2i_out_valid, i2f_in_ready : std_logic;
    signal f2i_out_data : std_logic_vector(8+23+2 downto 0);
begin

    f2i : float2int
        port map (clk => clk,
                  rst => rst,
                  op_in_ready_0 => op_in_ready_0,
                  op_in_valid_0 => op_in_valid_0,
                  op_in_data_0 => op_in_data_0,

                  op_out_ready_0 => i2f_in_ready,
                  op_out_valid_0 => f2i_out_valid,
                  op_out_data_0 => f2i_out_data );

    i2f : int2float
        port map (clk => clk,
                  rst => rst,
                  op_in_ready_0 => i2f_in_ready,
                  op_in_valid_0 => f2i_out_valid,
                  op_in_data_0 => f2i_out_data,

                  op_out_ready_0 => op_out_ready_0,
                  op_out_valid_0 => op_out_valid_0,
                  op_out_data_0 => op_out_data_0 );


end architecture;
