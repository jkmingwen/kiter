-- Takes in multiple inputs and routes them to the output ports depending
-- on the phase of execution defined by 'current_phase'.
library ieee;
use ieee.std_logic_1164.all;

entity input_selector is
  generic ( num_phases : integer );
  port ( clk        : in std_logic;
         rst      : in std_logic;

         op_in_ready_0 : out std_logic;
         op_in_valid_0 : in std_logic;
         op_in_data_0  : in std_logic_vector (33 downto 0);

         op_in_ready_1 : out std_logic;
         op_in_valid_1 : in std_logic;
         op_in_data_1  : in std_logic_vector (33 downto 0);

         op_out_ready_0  : in std_logic;
         op_out_valid_0  : out std_logic;
         op_out_data_0 : out std_logic_vector (33 downto 0) );
end input_selector;

architecture Behavioral of input_selector is
    signal current_phase, next_phase : integer := 0;

begin

  update_execution_phase : process(clk)
  begin
    if falling_edge(clk) then
        -- check out_ready_0 for both inputs as it corresponds to
        -- the input port for the AXI merger component
        -- check for valid signal on input port corresponding to execution phase
        if (current_phase = 0) then
            if (op_in_valid_0 = '1' and op_out_ready_0 = '1') then
                next_phase <= (current_phase + 1) mod num_phases;
            end if;
        elsif (current_phase = 1) then
            if (op_in_valid_1 = '1' and op_out_ready_0 = '1') then
                next_phase <= (current_phase + 1) mod num_phases;
            end if;
        end if;
        -- reset behaviour
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
                    op_in_valid_1 when (current_phase = 1) else
                    '0';
  op_out_data_0 <= op_in_data_0 when (current_phase = 0) else
                   op_in_data_1 when (current_phase = 1) else
                   "0000000000000000000000000000000000";
  op_in_ready_0 <= op_out_ready_0 when (current_phase = 0) else
                   '0';
  op_in_ready_1 <= op_out_ready_0 when (current_phase = 1) else
                   '0';

end Behavioral;
