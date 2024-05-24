-- Input selector (scheduled)
-- Takes in N inputs, and produces 1 output
-- At each exec_time_N, the input data on the respective port is written to output,
-- this value will not change until the next push time occurs

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity $COMPONENT_NAME is
    generic (ram_width : integer;
             $EXEC_TIME_PORTS
             );
    port (
        clk         : in  std_logic;
        rst       : in  std_logic;
        cycle_count : in integer;
        $INPUT_PORTS
        out_data_0    : out std_logic_vector(ram_width-1 downto 0));
end $COMPONENT_NAME;

architecture Behavioral of $COMPONENT_NAME is

begin

    select_input :  process (clk)
    begin
        if rising_edge(clk) then
            case (cycle_count) is
                $PROCESS_BEHAVIOUR
                when others =>
                    null;
            end case;
        end if;
        if (rst = '0') then
            out_data_0 <= (others => '0');
        end if;
    end process select_input;


end Behavioral;
