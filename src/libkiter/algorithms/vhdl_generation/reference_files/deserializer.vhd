library ieee;
use ieee.std_logic_1164.all;


entity deserializer is
    generic (
        ram_width : natural := 34;
        ram_depth : natural := 2);
    port (
        clk      : in std_logic;
        rst      : in std_logic;
        out_valid : out std_logic;
        in_data  : in std_logic;
        out_data : out std_logic_vector(ram_width-1 downto 0) := (others => '0'));
end deserializer;

architecture Behavioral of deserializer is
    signal shift_register : std_logic_vector(ram_width -1 downto 0) := (others => '0');
    signal bit_counter    : integer := 0;
    signal is_out_valid   : std_logic := '0';
begin
    collect_bits : process(clk)
    begin
        if falling_edge(clk) then
            if (bit_counter = ram_width - 1) then
                is_out_valid <= '1';
                bit_counter <= 0;
            else
                shift_register(bit_counter) <= in_data;
                bit_counter <= bit_counter + 1;
                is_out_valid <= '0';
            end if;
            if rst = '1' then
                bit_counter <= 0;
                is_out_valid <= '0';
            end if;
        end if;

        if rising_edge(clk) then
            out_valid <= is_out_valid;
            out_data <= shift_register;
        end if;
    end process collect_bits;

end Behavioral;
