# CircleMUD Security Baseline

CircleMUD is a network-facing server. Treat all client input, world data, and persistent records as untrusted or integrity-sensitive data.

## Immediate controls

- Run the server under a dedicated unprivileged OS account.
- Keep the executable and world data outside writable web-server directories.
- Restrict administrative and data directories with filesystem permissions.
- Use a host firewall and expose only the required game port.
- Back up player and world data regularly and test restoration.
- Rotate logs and avoid storing credentials or secrets in logs.
- Never commit passwords, private keys, production configuration, or player data.

## Code audit priorities

The first security review should concentrate on:

1. network input parsing;
2. fixed-size buffers and string operations;
3. format-string usage;
4. file/path construction from player-controlled input;
5. authentication and password handling;
6. privilege checks for administrative commands;
7. world-file parsing;
8. integer overflow and signed/unsigned conversions;
9. persistence corruption and partial writes;
10. random-number and combat calculations.

## Modernization rule

Security changes should be isolated from gameplay changes whenever possible. Each vulnerability fix should include a regression test or a documented reproduction when an automated test is impractical.

Do not assume that a clean compiler build means the server is safe to expose to the public Internet.
