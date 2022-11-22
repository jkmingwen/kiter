library ieee;
use ieee.std_logic_1164.all;


entity serializer is
    generic (
        ram_width : natural := 34;
        ram_depth : natural := 2);
    port (
        clk      : in std_logic;
        rst      : in std_logic;
        in_valid : in std_logic;
        in_data  : in std_logic_vector(ram_width-1 downto 0) := (others => '0');
        out_data : out std_logic );
end serializer;

architecture Behavioral of serializer is
    signal shift_register : std_logic_vector(ram_width -1 downto 0) := (others => '0');
    signal bit_counter    : integer := 0;
begin
    collect_bits : process(clk)
    begin
        if falling_edge(clk) then
            if (in_valid = '1') then -- store input float once valid signal is received
                shift_register <= in_data;
            end if;
            if (bit_counter = ram_width - 1) then
                bit_counter <= 0;
            else
                bit_counter <= bit_counter + 1;
            end if;
            if rst = '1' then
                bit_counter <= 0;
            end if;
        end if;

        if rising_edge(clk) then
            out_data <= shift_register(bit_counter);
        end if;
    end process collect_bits;

end Behavioral;
