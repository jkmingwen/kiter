library ieee;
use ieee.std_logic_1164.all;

entity vslider is
   generic ( default_value : std_logic_vector (33 downto 0)
             -- max_value : std_logic_vector (33 downto 0);
             -- min_value : std_logic_vector (33 downto 0)
             );
   port ( clk : in std_logic;
          rst : in std_logic;
          op_in_data_0 : in std_logic_vector (33 downto 0) := (others => '0');
          op_out_ready_0 : in std_logic;
          op_out_valid_0 : out std_logic;
          op_out_data_0 : out std_logic_vector (33 downto 0) := (others => '0')
);
end vslider;

architecture Behavioral of vslider is
signal is_out_valid : std_logic := '0';
signal temp_data : std_logic_vector (33 downto 0) := default_value;
begin

reset_behaviour : process(clk)
begin
   if falling_edge(clk) then
       if (rst = '0') then
           is_out_valid <= '0';
           temp_data <= default_value;
       else
         -- TODO implement max/min limiting behaviour
           is_out_valid <= '1';
           temp_data <= op_in_data_0;
       end if;
   end if;
end process;

write_data : process(clk) -- write stored data to output ports
 begin
   if rising_edge(clk) then
       op_out_valid_0 <= is_out_valid;
       op_out_data_0 <= temp_data;
   end if;
 end process write_data;



end Behavioral;
