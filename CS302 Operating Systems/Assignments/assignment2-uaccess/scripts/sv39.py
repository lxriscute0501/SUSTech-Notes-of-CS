# This script calculates the three VPNs and page offset for a given RISC-V Sv39 address.
# Sv39 uses a 39-bit virtual address space, divided as follows:
# - VPN[2]: bits 38-30
# - VPN[1]: bits 29-21
# - VPN[0]: bits 20-12
# - Page Offset: bits 11-0

def validate_sv39_address(virtual_address):
    """
    Validate that the given virtual address conforms to Sv39 rules.
    If the 38th bit is 1, all higher bits (63:39) must also be 1.
    If the 38th bit is 0, all higher bits (63:39) must also be 0.
    """
    bit38 = (virtual_address >> 38) & 1
    if bit38:
        print("Bit38 == 1")
        higher_addr = virtual_address >> 38
        if higher_addr != (0xFFFFFFC000000000 >> 38):
            raise ValueError(
                "Invalid Sv39 address: higher bits do not match the 38th bit.")


def calculate_vpn_and_offset(virtual_address):
    """
    Calculate VPN[2], VPN[1], VPN[0], and page offset for a given Sv39 virtual address.
    """
    # Validate the address first
    validate_sv39_address(virtual_address)

    # Masks for extracting VPNs and page offset
    vpn_mask = 0x1FF  # 9 bits
    page_offset_mask = 0xFFF  # 12 bits

    # Extract VPNs and page offset
    page_offset = virtual_address & page_offset_mask
    vpn0 = (virtual_address >> 12) & vpn_mask
    vpn1 = (virtual_address >> 21) & vpn_mask
    vpn2 = (virtual_address >> 30) & vpn_mask

    return vpn2, vpn1, vpn0, page_offset


def decode_sv39_pte(pte):
    ppn = (pte >> 10) << 12  # PPN spans bits 10-53
    v = pte & (1 << 0)
    r = pte & (1 << 1)
    w = pte & (1 << 2)
    x = pte & (1 << 3)
    u = pte & (1 << 4)
    g = pte & (1 << 5)
    a = pte & (1 << 6)
    d = pte & (1 << 7)

    print(f"Phys Addr: 0x{ppn:X}, {'D' if d else '-'}{'A' if a else '-'}{'G' if g else '-'}{
          'U' if u else '-'}{'X' if x else '-'}{'W' if w else '-'}{'R' if r else '-'}{'V' if v else '-'}")


if __name__ == "__main__":
    # Example: Get user input for virtual address
    cmd = str(
        input("Action: \n\t1. Check Sv39 VAddr \n\t2. Explain PTE\n> ")).strip()
    if cmd == '2':
        pte_input = input(
            "Enter a 64-bit Sv39 PTE in hexadecimal (e.g., 0x123456789ABCDEF): ")
        pte = int(pte_input, 16)
        if pte >= (1 << 64):
            raise ValueError("PTE exceeds 64-bit range.")
        # Decode the PTE
        decode_sv39_pte(pte)
    else:
        try:
            virtual_address = int(input(
                "Enter a RISC-V Sv39 virtual address (in hexadecimal, e.g., 0x123456789): "), 16)

            if virtual_address >= (1 << 64):
                raise ValueError("Address exceeds 64-bit range.")

            vpn2, vpn1, vpn0, page_offset = calculate_vpn_and_offset(
                virtual_address)

            print(f"Virtual Address: 0x{virtual_address:X}")
            print(f"VPN[2]: {vpn2} (0b{vpn2:09b})")
            print(f"VPN[1]: {vpn1} (0b{vpn1:09b})")
            print(f"VPN[0]: {vpn0} (0b{vpn0:09b})")
            print(f"Page Offset: {page_offset} (0b{page_offset:012b})")

        except ValueError as e:
            print(f"Invalid input: {e}")
