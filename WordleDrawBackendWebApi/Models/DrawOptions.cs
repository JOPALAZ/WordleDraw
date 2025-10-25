namespace WordleDrawBackendWebApi.Models
{
    enum ValidatorType
	{
		PerferctValidator,
		PresenceValidator,
		MixedValidator,
		FallbackPresenceValidator,
		FallbackMixedValidator,
		AtLeastGreenValidator,
		AtLeastYellowValidator,
		NotSpecified
	};
public class DrawOptions
    {
        public string enumValue { get; set; }
		public string answer { get; set; }
        public ICollection<ICollection<int>> grid { get; set; }
    }
}
