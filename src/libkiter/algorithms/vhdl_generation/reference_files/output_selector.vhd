-- Takes in two inputs, sending to output at the
-- same time. Will store input if they don't arrive at the same time; will only
-- output once both inputs are provided.
library ieee;
use ieee.std_logic_1164.all;

entity output_selector is
  generic ( num_phases : integer );
  port ( clk        : in std_logic;
         rst      : in std_logic;

         op_in_ready_0 : out std_logic;
         op_in_valid_0 : in std_logic;
         op_in_data_0  : in std_logic_vector (33 downto 0);

         op_out_ready_0  : in std_logic;
         op_out_valid_0  : out std_logic;
         op_out_data_0 : out std_logic_vector (33 downto 0);

         op_out_ready_1  : in std_logic;
         op_out_valid_1  : out std_logic;
         op_out_data_1 : out std_logic_vector (33 downto 0) );
end output_selector;

architecture Behavioral of output_selector is
    signal current_phase, next_phase : integer := 0;


begin

  update_execution_phase : process(clk)
  begin
    if falling_edge(clk) then
        if (current_phase = 0) then
            if (op_in_valid_0 = '1' and op_out_ready_0 = '1') then
                next_phase <= (current_phase + 1) mod num_phases;
            end if;
        elsif (current_phase = 1) then
            if (op_in_valid_0 = '1' and op_out_ready_1 = '1') then
                next_phase <= (current_phase + 1) mod num_phases;
            end if;
        end if;
        -- rst behaviour
        if (rst = '0') then
            current_phase <= 0;
            next_phase <= 0;
        end if;
    end if;

    if rising_edge(clk) then
        current_phase <= next_phase;
    end if;
  end process update_execution_phase;

  op_out_valid_0 <= op_in_valid_0 when (current_phase = 0) else
                    '0';
  op_out_data_0 <= op_in_data_0 when (current_phase = 0) else
                   "0000000000000000000000000000000000";
  op_out_valid_1 <= op_in_valid_0 when (current_phase = 1) else
                    '0';
  op_out_data_1 <= op_in_data_0 when (current_phase = 1) else
                   "0000000000000000000000000000000000";
  op_in_ready_0 <= op_out_ready_0 when (current_phase = 0) else
                   op_out_ready_1 when (current_phase = 1) else
                   '0';

end Behavioral;
